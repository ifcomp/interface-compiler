#include "Model/Class/Event.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::Event::TYPE_NAME = "Event";

Class::Event::Event()
{
}

Class::Event::~Event()
{
}

ObjectRef Class::Event::clone() const
{
    EventRef newEvent = std::make_shared<Event>();
    clone(newEvent);
    return newEvent;
}

void Class::Event::addValue(ParameterRef value)
{
    _values.push_back(value);
}

std::vector<ParameterRef> Class::Event::values() const
{
    return _values;
}

void Class::Event::setTypeId(boost::uuids::uuid typeId)
{
	_typeId = typeId;
}

boost::uuids::uuid Class::Event::typeId() const
{
    return _typeId;
}

void Class::Event::clone(ObjectRef clonedObject) const
{
    using namespace std;

    EventRef clonedEvent = dynamic_pointer_cast<Event>(clonedObject);

    if (clonedEvent)
    {
        Identifiable::clone(clonedEvent);
        clonedEvent->setTypeId(typeId());

        for (auto value : values())
        {
            clonedEvent->addValue(dynamic_pointer_cast<Parameter>(value->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Class - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
