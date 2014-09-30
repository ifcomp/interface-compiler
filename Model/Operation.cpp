#include "Model/Operation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Operation::TYPE_NAME = "Operation";

Operation::Operation() :
    _isStatic(false),
    _isSynchronous(false)
{
}

Operation::~Operation()
{
}

bool Operation::isStatic()
{
	return _isStatic;
}

void Operation::setStatic(bool isStatic)
{
	_isStatic = isStatic;
}

bool Operation::isSynchronous()
{
	return _isSynchronous;
}

void Operation::setSynchronous(bool isSynchronous)
{
	_isSynchronous = isSynchronous;
}

void Operation::addParam(ParameterRef param)
{
    _params[param->longName()] = param;
}

std::vector<ParameterRef> Operation::params()
{
    return mapToVector<ParameterRef>(_params);
}

void Operation::setResult(ParameterRef result)
{
	_result = result;
}

ParameterRef Operation::result()
{
	return _result;
}

} } } // namespace Everbase::InterfaceCompiler::Model