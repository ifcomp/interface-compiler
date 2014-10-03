#include "Model/Struct.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Struct::TYPE_NAME = "Struct";

Struct::Struct()
{
}

Struct::~Struct()
{
}

void Struct::addField(ParameterRef field)
{
    _fields.push_back(field);
}

std::vector<ParameterRef> Struct::fields()
{
    return _fields;
}

} } } // namespace Everbase::InterfaceCompiler::Model
