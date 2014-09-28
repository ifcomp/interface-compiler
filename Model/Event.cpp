#include "Model/Event.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

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

void Event::addResult(ParameterRef result)
{
    mResults[result->longName()] = result;
}

const std::map<std::string, ParameterRef>& Event::results()
{
	return mResults;
}

} } } // namespace Everbase::InterfaceCompiler::Model
