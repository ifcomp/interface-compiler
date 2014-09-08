#include "model/operation.hpp"

using namespace Api::Model;

Operation::Operation()
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

void Operation::addParam(ParameterPtr param)
{
    mParams[param->longName()] = param;
}

const std::map<std::string, ParameterPtr>& Operation::params()
{
	return mParams;
}

void Operation::setResult(ParameterPtr result)
{
	mResult = result;
}

ParameterPtr Operation::result()
{
	return mResult;
}
