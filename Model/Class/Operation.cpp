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

bool Class::Operation::isStatic()
{
	return _isStatic;
}

void Class::Operation::setStatic(bool isStatic)
{
	_isStatic = isStatic;
}

bool Class::Operation::isSynchronous()
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

std::vector<ParameterRef> Class::Operation::params()
{
    return _params;
}

void Class::Operation::setResult(ParameterRef result)
{
	_result = result;
}

ParameterRef Class::Operation::result()
{
	return _result;
}

} } } // namespace Everbase::InterfaceCompiler::Model
