#include "struct.hpp"

using namespace Api::Model;

Struct::Struct()
{
}

Struct::~Struct()
{
}

void Struct::addField(ParameterPtr field)
{
    mFields[field->longName()] = field;
}

const std::map<std::string, ParameterPtr>& Struct::fields()
{
	return mFields;
}
