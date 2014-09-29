#include "Model/Class.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Class::Class()
{
}

Class::~Class()
{
}

Class::ClassType Class::type()
{
	return _Type;
}

void Class::setType(Class::ClassType type)
{
    _Type = type;
}

TypeRef Class::parent()
{
    return _Parent;
}

void Class::setParent(TypeRef parent)
{
	_Parent = parent;
}

void Class::addOperation(OperationRef operation)
{
    _operations[operation->longName()] = operation;
}

std::vector<OperationRef> Class::operations()
{
	std::vector<OperationRef> result;

	for(auto operation : _operations)
		{ result.push_back(operation.second); }

	return result;
}

void Class::addEvent(EventRef event)
{
    _events[event->longName()] = event;
}

const std::map<std::string, EventRef>& Class::events()
{
	return _events;
}

} } } // namespace Everbase::InterfaceCompiler::Model
