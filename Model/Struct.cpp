#include "Model/Struct.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

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

const std::map<std::string, ParameterRef>& Struct::fields()
{
	return _fields;
}

} } } // namespace Everbase::InterfaceCompiler::Model
