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
    mValues[value->longName()] = value;
}

const std::map<std::string, ValueRef>& Enum::values()
{
	return mValues;
}

} } } // namespace Everbase::InterfaceCompiler::Model
