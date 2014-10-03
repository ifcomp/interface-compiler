#include "Model/Class.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::TYPE_NAME = "Class";

Class::Class()
	: _type(ClassType::INTERFACE)
{
}

Class::~Class()
{
}

Class::ClassType Class::type()
{
	return _type;
}

void Class::setType(Class::ClassType type)
{
    _type = type;
}

TypeBaseRef Class::parent()
{
    return _parent;
}

void Class::setParent(TypeBaseRef parent)
{
	_parent = parent;
}

void Class::addOperation(OperationRef operation)
{
    _operations.push_back(operation);
}

std::vector<OperationRef> Class::operations()
{
	return _operations;
}

void Class::addEvent(EventRef event)
{
    _events.push_back(event);
}

std::vector<EventRef> Class::events()
{
    return _events;
}

} } } // namespace Everbase::InterfaceCompiler::Model
