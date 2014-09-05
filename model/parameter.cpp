#include "model/parameter.hpp"

using namespace Api::Model;

Parameter::Parameter()
{
}

Parameter::~Parameter()
{
}

TypePtr Parameter::type()
{
	return mType;
}

void Parameter::setType(TypePtr type)
{
	mType = type;
}
