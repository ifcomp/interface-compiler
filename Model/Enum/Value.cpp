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
    ValueRef newValue = std::make_shared<Value>();
    clone(newValue);
    return newValue;
}

int32_t Enum::Value::value() const
{
	return _value;
}

void Enum::Value::setValue(int32_t value)
{
    _value = value;
}

void Enum::Value::clone(ObjectRef clonedObject) const
{
    using namespace std;

    ValueRef clonedValue = dynamic_pointer_cast<Value>(clonedObject);

    if (clonedValue)
    {
        clonedValue->setValue(value());
    }
    else
    {
        throw runtime_error("clone() failed: expected Value - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
