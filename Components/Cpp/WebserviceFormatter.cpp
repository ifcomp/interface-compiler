#include "Components/Cpp/WebserviceFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void WebserviceFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// " << name(class_) << ": {" << endl << endl;

    if(class_->operations().size() > 0)
    {
        for( auto operation : class_->operations() )
        {
            stream << definition(operation) << endl;
        }
    }

    stream
        << name(class_) << "Impl::" << name(class_) << "Impl() { }" << endl
        << name(class_) << "Impl::~" << name(class_) << "Impl() { }" << endl << endl;

    stream << "// " << name(class_) << ": }" << endl << endl;
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
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

    stream << "}" << endl;
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void WebserviceFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
