#include "value.hpp"

using namespace Api::Model;

Value::Value()
{
}

Value::~Value()
{
}

int32_t Value::value()
{
	return mValue;
}

void Value::setValue(int32_t value)
{
	mValue = value;
}
