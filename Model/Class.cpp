#include "Model/Class.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::TYPE_NAME = "Class";

Class::Class()
	: _behavior(Behavior::INTERFACE)
{
}

Class::~Class()
{
}

Class::Behavior Class::behavior()
{
	return _behavior;
}

void Class::setBehavior(Class::Behavior behavior)
{
    _behavior = behavior;
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

void Class::addConstant(ConstantRef constant)
{
	_constants.push_back(constant);
}

std::vector<ConstantRef> Class::constants()
{
	return _constants;
}

} } } // namespace Everbase::InterfaceCompiler::Model
