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
	return _value;
}

void Value::setValue(int32_t value)
{
	_value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
