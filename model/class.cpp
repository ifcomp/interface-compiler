#include "model/class.hpp"

using namespace Api::Model;

Class::Class()
{
}

Class::~Class()
{
}

Class::ClassType Class::type()
{
	return mType;
}

void Class::setType(Class::ClassType type)
{
	mType = type;
}

std::shared_ptr<Class> Class::parent()
{
	return mParent;
}

void Class::setParent(std::shared_ptr<Class> parent)
{
	mParent = parent;
}

void Class::addOperation(OperationPtr operation)
{
	mOperations[operation->name().longName()] = operation;
}

const std::map<std::string, OperationPtr>& Class::operations()
{
	return mOperations;
}

void Class::addEvent(EventPtr event)
{
	mEvents[event->name().longName()] = event;
}

const std::map<std::string, EventPtr>& Class::events()
{
	return mEvents;
}
