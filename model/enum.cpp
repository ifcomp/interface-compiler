#include "enum.hpp"

using namespace Api::Model;

Enum::Enum()
{
}

Enum::~Enum()
{
}

void Enum::addValue(ValuePtr value)
{
	mValues[value->name().longName()] = value;
}

const std::map<std::string, ValuePtr>& Enum::values()
{
	return mValues;
}
