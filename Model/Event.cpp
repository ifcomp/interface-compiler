#include "Model/Event.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Event::Event() :
    _isStatic(false)
{
}

Event::~Event()
{
}

bool Event::isStatic()
{
    return _isStatic;
}

void Event::setStatic(bool isStatic)
{
    _isStatic = isStatic;
}

void Event::addResult(ParameterRef result)
{
    _results[result->longName()] = result;
}

const std::map<std::string, ParameterRef>& Event::results()
{
    return _results;
}

} } } // namespace Everbase::InterfaceCompiler::Model
