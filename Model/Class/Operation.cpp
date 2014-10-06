#include "Model/Class/Operation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::Operation::TYPE_NAME = "Operation";

Class::Operation::Operation() :
    _isStatic(false),
    _isSynchronous(false)
{
}

Class::Operation::~Operation()
{
}

ObjectRef Class::Operation::clone() const
{
    OperationRef newOperation = std::make_shared<Operation>();
    clone(newOperation);
    return newOperation;
}

bool Class::Operation::isStatic() const
{
	return _isStatic;
}

void Class::Operation::setStatic(bool isStatic)
{
	_isStatic = isStatic;
}

bool Class::Operation::isSynchronous() const
{
	return _isSynchronous;
}

void Class::Operation::setSynchronous(bool isSynchronous)
{
	_isSynchronous = isSynchronous;
}

void Class::Operation::addParam(ParameterRef param)
{
    _params.push_back(param);
}

std::vector<ParameterRef> Class::Operation::params() const
{
    return _params;
}

void Class::Operation::setResult(ParameterRef result)
{
	_result = result;
}

ParameterRef Class::Operation::result() const
{
    return _result;
}

void Class::Operation::clone(ObjectRef clonedObject) const
{
    using namespace std;

    OperationRef clonedOp = dynamic_pointer_cast<Operation>(clonedObject);

    if (clonedOp)
    {
        Identifiable::clone(clonedOp);
        clonedOp->setStatic(isStatic());
        clonedOp->setSynchronous(isSynchronous());
        clonedOp->setResult(dynamic_pointer_cast<Parameter>(result()->clone()));

        for (auto param : params())
        {
            clonedOp->addParam(dynamic_pointer_cast<Parameter>(param->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Operation - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
