#include "model/event.hpp"

using namespace Api::Model;

Event::Event()
{
}

Event::~Event()
{
}

void Event::addParam(ParameterPtr param)
{
    mParams[param->longName()] = param;
}

const std::map<std::string, ParameterPtr>& Event::params()
{
	return mParams;
}

void Event::addResult(ParameterPtr result)
{
    mResults[result->longName()] = result;
}

const std::map<std::string, ParameterPtr>& Event::results()
{
	return mResults;
}
