#include "Components/Ruby/EventsFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void EventsFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);
}

void EventsFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

    for ( auto element : namespace_->elements() )
    {
        stream << definition(element);
    }

    stream << "// namespace " << qname(namespace_) << ": }" << endl << endl;
}

void EventsFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void EventsFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    for( auto event : class_->events() )
    {
        stream << definition(event) << endl;
    }
}

void EventsFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void EventsFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(event->parent());

    if(!class_)
        throw std::runtime_error("invalid event");

    if ( event->doc() )
    {
        stream << doc(event->doc()) << endl;
    }

    stream << "// event " << qname(event) << ": {" << endl << endl;

    stream << "EVERBASE_RUBY_EVENT(" << qcname(event, "_") << ", " << qcname(class_, "_") << ", \"" << name(event) << "\");" << endl << endl;

    for( auto value : event->values() )
    {
        stream << "EVERBASE_RUBY_EVENT_VALUE(" << qcname(value, "_") << ", " << qcname(event, "_") << ", \"" << name(value) << "\");" << endl;
    }

    stream << endl;

    stream << "// event " << qname(event) << ": }" << endl << endl;
}

void EventsFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void EventsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void EventsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
