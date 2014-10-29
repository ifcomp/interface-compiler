#include "Components/ObjectiveC/WrapperFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace ObjectiveC {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void WrapperFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include \"everbase/everbase.h\"" << endl
           << endl;
}

void WrapperFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// " << qname(class_) << ": {" << endl << endl;

    stream << "@implementation " << qname(class_) << endl;

    if(class_->operations().size() > 0)
    {
        for( auto operation : class_->operations() )
        {
            stream << definition(operation) << endl;
        }
    }

    stream
        << "- (id) init {" << endl << "}" << endl << endl
        << "- (id) initWithData:(void*)data {" << endl << "}" << endl << endl
        << "- (void) dealloc {" << endl << "}" << endl << endl
        << "- (void*) data {" << endl << "}" << endl << endl;

    stream << "@end" << endl;

    for( auto constant : class_->constants() )
    {
        stream << definition(constant) << endl;
    }

    if(class_->events().size() > 0)
    {
        for( auto event : class_->events() )
        {
            stream << definition(event) << endl;
        }
    }

    stream << "// " << qname(class_) << ": }" << endl << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    auto class_ = std::dynamic_pointer_cast<Class>(constant->parent());

    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "const " << type(constant->type()) << " " << qname(class_) << name(constant) << " = ";

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
                stream << "@\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "[[NSUUID alloc] initWithUUIDBytes:(uuid_t){ ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " }]";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

    stream << ";" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Class>(event->parent());

    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "const NSString* " << qname(class_) << name(event) << "TypeName = @\"" << qcname(event) << "\";" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("parent is not a class");

    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation);

    stream << " {" << endl;

    stream << "}" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::ObjectiveC
