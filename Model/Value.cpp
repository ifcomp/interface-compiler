#include "Model/Value.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

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

} } } // namespace Everbase::InterfaceCompiler::Model
