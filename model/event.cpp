#include "model/event.hpp"

using namespace Api::Model;

Event::Event() :
    mIsStatic(false)
{
}

Event::~Event()
{
}

bool Event::isStatic()
{
    return mIsStatic;
}

void Event::setStatic(bool isStatic)
{
    mIsStatic = isStatic;
}

void Event::addResult(ParameterPtr result)
{
    mResults[result->longName()] = result;
}

const std::map<std::string, ParameterPtr>& Event::results()
{
	return mResults;
}
