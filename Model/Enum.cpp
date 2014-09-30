#include "Model/Enum.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Enum::TYPE_NAME = "Enum";

Enum::Enum()
{
}

Enum::~Enum()
{
}

void Enum::addValue(ValueRef value)
{
    _values[value->longName()] = value;
}

std::vector<ValueRef> Enum::values()
{
    return mapToVector<ValueRef>(_values);
}

} } } // namespace Everbase::InterfaceCompiler::Model
