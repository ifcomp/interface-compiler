#include "Components/Cpp/LibraryFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void LibraryFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include \"library/library.hpp\"" << endl
           << endl;
}

void LibraryFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
}

void LibraryFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void LibraryFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// " << name(class_) << ": {" << endl << endl;

    for( auto constant : class_->constants() )
    {
        stream << definition(constant) << endl;
    }

    if(class_->operations().size() > 0)
    {
        for( auto operation : class_->operations() )
        {
            stream << definition(operation) << endl;
        }
    }

    if(class_->events().size() > 0)
    {
        for( auto event : class_->events() )
        {
            stream << definition(event) << endl;
        }
    }

    // constructor
    stream << name(class_) << "Impl::" << name(class_) << "Impl(const everbase::internal::common::rpc::ObjectDirectory::HandleT& handle) : handle_(handle) { }" << endl << endl;

    // destructor
    stream << name(class_) << "Impl::~" << name(class_) << "Impl()" << endl << "{" << endl;
    filter(stream).push<indent>()
        << "if(!everbase::internal::library::client)" << endl
        << "{" << endl
        << "    return;" << endl
        << "}" << endl << endl
        << "try" << endl
        << "{" << endl
        << "    everbase::internal::library::client->destroyProxyObject"
        << "(\"" << qcname(class_) << "::~" << name(class_) << "\", handle_);" << endl
        << "}" << endl
        << "catch(const std::exception& e)" << endl
        << "{" << endl
        << "    throw e;" << endl
        << "}" << endl;
    stream << "}" << endl << endl;

    stream << "// " << name(class_) << ": }" << endl << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "const " << type(constant->type()) << " " << qname(constant) << " = ";

    if( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
                stream << "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
                stream << (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "{ { ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " } }";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

    stream << ";" << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "const char " << qname(event) << "::TYPE_NAME[] = \"" << qcname(event) << "\";" << endl;
    stream << "const bool " << qname(event) << "::TYPE_EXTERNAL = true;" << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("parent is not a class");

    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    if (operation->result())
    {
        stream << type(operation->result()->type());
    }
    else
    {
        stream << "void";
    }

    stream << " " << name(class_) << (!operation->isStatic() ? "Impl" : "") << "::" << name(operation) << "(";

    for (auto parameter : indices(operation->params()))
    {
        stream << param(parameter.value()) << (!parameter.last() ? ", " : "");
    }

    stream << ")" << (operation->isConst() ? " const" : "") << endl << "{" << endl;

    filter(stream).push<indent>()
           << "if(!everbase::internal::library::client)" << endl
           << "    { throw std::runtime_error(\"no connection available\"); }" << endl;

    if (operation->result())
    {
        filter(stream).push<indent>() << "return ";
    }

    stream << "everbase::internal::library::client->call<";

    if (operation->result())
    {
        stream << type(operation->result()->type());
    }
    else
    {
        stream << "void";
    }

    if(!operation->isStatic())
    {
        stream << ", std::shared_ptr<" << (operation->isConst() ? "const " : "") << qname(class_) << ">";
    }

    for (auto parameter : operation->params())
    {
        stream << ", " << type(parameter->type());
    }

    stream << ">(\"" << qcname(operation) << "\"";

    if(!operation->isStatic())
    {
        stream << ", std::dynamic_pointer_cast<" << (operation->isConst() ? "const " : "") << qname(class_) << ">(shared_from_this())";
    }

    for (auto parameter : operation->params())
    {
        stream << ", std::move(" << name(parameter) << ")";
    }

    stream << ");" << endl;

    stream << "}" << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
