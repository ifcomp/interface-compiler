#include "Model/Enum/Value.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Enum::Value::TYPE_NAME = "Value";

Enum::Value::Value()
{
}

Enum::Value::~Value()
{
}

ObjectRef Enum::Value::clone() const
{
    ValueRef clonedValue = std::make_shared<Value>();
    clonedValue->setValue(value());
    return clonedValue;
}

int32_t Enum::Value::value() const
{
	return _value;
}

void Enum::Value::setValue(int32_t value)
{
	_value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
