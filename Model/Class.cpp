#include "Model/Class.hpp"
#include "Model/Class/Operation.hpp"
#include "Model/Class/Event.hpp"
#include "Model/Class/Constant.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::TYPE_NAME = "Class";

Class::Class()
//	: _behavior(Behavior::INTERFACE)
{
}

Class::~Class()
{
}

ObjectRef Class::clone() const
{
    using namespace std;

    ClassRef newClass = std::make_shared<Class>();
    clone(newClass);
    return newClass;
}

//Class::Behavior Class::behavior() const
//{
//	return _behavior;
//}

//void Class::setBehavior(Class::Behavior behavior)
//{
//    _behavior = behavior;
//}

TypeBaseRef Class::super() const
{
    return _super;
}

void Class::setSuper(TypeBaseRef super)
{
    super->setParent(shared_from_this());
	_super = super;
}

void Class::addOperation(Class::OperationRef operation)
{
    operation->setParent(shared_from_this());
    _operations.push_back(operation);
}

std::vector<Class::OperationRef> Class::operations() const
{
	return _operations;
}

void Class::addEvent(Class::EventRef event)
{
    event->setParent(shared_from_this());
    _events.push_back(event);
}

std::vector<Class::EventRef> Class::events() const
{
    return _events;
}

void Class::addConstant(Class::ConstantRef constant)
{
    constant->setParent(shared_from_this());
	_constants.push_back(constant);
}

std::vector<Class::ConstantRef> Class::constants() const
{
    return _constants;
}

void Class::clone(ObjectRef clonedObject) const
{
    using namespace std;

    ClassRef clonedClass = dynamic_pointer_cast<Class>(clonedObject);

    if (clonedClass)
    {
        Identifiable::clone(clonedClass);

        //clonedClass->setBehavior(behavior());

        if (super())
        {
            clonedClass->setSuper(dynamic_pointer_cast<TypeBase>(super()->clone()));
        }

        for (auto operation : _operations)
        {
            clonedClass->addOperation(dynamic_pointer_cast<Operation>(operation->clone()));
        }

        for (auto event : _events)
        {
            clonedClass->addEvent(dynamic_pointer_cast<Event>(event->clone()));
        }

        for (auto constant : _constants)
        {
            clonedClass->addConstant(dynamic_pointer_cast<Constant>(constant->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Class - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
