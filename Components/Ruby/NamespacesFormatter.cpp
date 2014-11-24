#include "Components/Ruby/NamespacesFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void NamespacesFormatter::_includes(std::ostream& stream) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    Model::NamespaceRef parent;

    if(namespace_->parent())
    {
        parent = std::dynamic_pointer_cast<Model::Namespace>(namespace_->parent());
    }

    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc()) << endl;
    }

    stream << "// namespace " << qname(namespace_) << ": {" << endl << endl;

    if(parent)
    {
        stream << "EVERBASE_RUBY_NAMESPACE(" << qcname(namespace_, "_") << ", " << qcname(parent, "_") << ", \"" << name(namespace_) << "\")" << endl << endl;
    }
    else
    {
        stream << "EVERBASE_RUBY_NAMESPACE_ROOT(" << qcname(namespace_, "_") << ", \"" << name(namespace_) << "\")" << endl << endl;
    }

    for ( auto element : namespace_->elements() )
    {
        stream << definition(element);
    }

    stream << "// namespace " << qname(namespace_) << ": }" << endl << endl;
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void NamespacesFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
