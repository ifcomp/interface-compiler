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
    mFields[field->longName()] = field;
}

const std::map<std::string, ParameterRef>& Struct::fields()
{
	return mFields;
}

} } } // namespace Everbase::InterfaceCompiler::Model
