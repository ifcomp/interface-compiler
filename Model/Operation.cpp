#include "Model/Operation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Operation::Operation() :
    mIsStatic(false),
    mIsSynchronous(false)
{
}

Operation::~Operation()
{
}

bool Operation::isStatic()
{
	return mIsStatic;
}

void Operation::setStatic(bool isStatic)
{
	mIsStatic = isStatic;
}

bool Operation::isSynchronous()
{
	return mIsSynchronous;
}

void Operation::setSynchronous(bool isSynchronous)
{
	mIsSynchronous = isSynchronous;
}

void Operation::addParam(ParameterRef param)
{
    mParams[param->longName()] = param;
}

std::vector<ParameterRef> Operation::params()
{
	std::vector<ParameterRef> result;

	for( auto param : mParams )
	{
		result.push_back(param.second);
	}

	return result;
}

void Operation::setResult(ParameterRef result)
{
	mResult = result;
}

ParameterRef Operation::result()
{
	return mResult;
}

} } } // namespace Everbase::InterfaceCompiler::Model
