#include "Model/Enum.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Enum::Enum()
{
}

Enum::~Enum()
{
}

void Enum::addValue(ValueRef value)
{
    _values[value->longName()] = value;
}

const std::map<std::string, ValueRef>& Enum::values()
{
	return _values;
}

} } } // namespace Everbase::InterfaceCompiler::Model
