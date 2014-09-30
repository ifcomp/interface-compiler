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
    _fields[field->longName()] = field;
}

std::vector<ParameterRef> Struct::fields()
{
    return mapToVector<ParameterRef>(_fields);
}

} } } // namespace Everbase::InterfaceCompiler::Model
