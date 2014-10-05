#include "Model/Enum.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Enum::TYPE_NAME = "Enum";

Enum::Enum()
{
}

Enum::~Enum()
{
}

void Enum::addValue(Enum::ValueRef value)
{
    _values.push_back(value);
}

std::vector<Enum::ValueRef> Enum::values()
{
    return _values;
}

} } } // namespace Everbase::InterfaceCompiler::Model
