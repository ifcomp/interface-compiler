#include "Components/Cpp/LibraryFormatter.hpp"

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

    stream
        << name(class_) << "Impl::" << name(class_) << "Impl()" << "{ }" << endl << endl
        << name(class_) << "Impl::~" << name(class_) << "Impl()" << endl << "{ }" << endl << endl;

    stream << "// " << name(class_) << ": }" << endl << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "constexpr " << type(constant->type()) << " " << qname(constant) << ";" << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "constexpr char " << qname(event) << "::TYPE_NAME[];" << endl;
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

    stream << ")" << endl << "{" << endl;

    if (operation->result())
    {
        filter(stream).push<indent>() << "return ";
    }

    filter(stream).push<indent>() << "everbase::internal::library::client->call<";

    if (operation->result())
    {
        filter(stream).push<indent>() << type(operation->result()->type());
    }
    else
    {
        filter(stream).push<indent>() << type("void");
    }

    if(!operation->isStatic())
    {
        filter(stream).push<indent>() << ", " << type(class_);
    }

    for (auto parameter : operation->params())
    {
        filter(stream).push<indent>() << ", " << type(parameter->type());
    }

    filter(stream).push<indent>() << ">(\"" << qcname(operation) << "\", ";

    if(!operation->isStatic())
    {
        filter(stream).push<indent>() << ", std::dynamic_pointer_cast<" << qname(class_) << ">(shared_from_this())";
    }

    for (auto parameter : operation->params())
    {
        filter(stream).push<indent>() << ", std::move(" << name(parameter) << ")";
    }

    filter(stream).push<indent>() << ");" << endl;

    stream << "}" << endl;
}

void LibraryFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void LibraryFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
