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
	return mType;
}

void Class::setType(Class::ClassType type)
{
    mType = type;
}

TypeRef Class::parent()
{
    return mParent;
}

void Class::setParent(TypeRef parent)
{
	mParent = parent;
}

void Class::addOperation(OperationRef operation)
{
    mOperations[operation->longName()] = operation;
}

std::vector<OperationRef> Class::operations()
{
	std::vector<OperationRef> result;

	for(auto operation : mOperations)
		{ result.push_back(operation.second); }

	return result;
}

void Class::addEvent(EventRef event)
{
    mEvents[event->longName()] = event;
}

const std::map<std::string, EventRef>& Class::events()
{
	return mEvents;
}

} } } // namespace Everbase::InterfaceCompiler::Model
