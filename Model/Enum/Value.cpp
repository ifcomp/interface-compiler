#include "Model/Enum/Value.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Enum::Value::TYPE_NAME = "Value";

Enum::Value::Value()
{
}

Enum::Value::~Value()
{
}

int32_t Enum::Value::value()
{
	return _value;
}

void Enum::Value::setValue(int32_t value)
{
	_value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
