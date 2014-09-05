#include "primitive.hpp"

using namespace Api::Model;

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
}

Primitive::PrimitiveType Primitive::type()
{
	return mType;
}

void Primitive::setType(Primitive::PrimitiveType type)
{
	mType = type;
}
