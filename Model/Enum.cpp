#include "Model/Enum.hpp"
#include "Model/Enum/Value.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Enum::TYPE_NAME = "Enum";

Enum::Enum()
{
}

Enum::~Enum()
{
}

ObjectRef Enum::clone() const
{
    EnumRef newEnum = std::make_shared<Enum>();
    clone(newEnum);
    return newEnum;
}

void Enum::addValue(Enum::ValueRef value)
{
    _values.push_back(value);
}

std::vector<Enum::ValueRef> Enum::values() const
{
    return _values;
}

void Enum::clone(ObjectRef clonedObject) const
{
    using namespace std;

    EnumRef clonedEnum = dynamic_pointer_cast<Enum>(clonedObject);

    if (clonedEnum)
    {
        Identifiable::clone(clonedEnum);

        for (auto value : values())
        {
            clonedEnum->addValue(dynamic_pointer_cast<Value>(value->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Enum - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
