#include "Model/Parameter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Parameter::Parameter()
{
}

Parameter::~Parameter()
{
}

TypeBaseRef Parameter::type()
{
	return _type;
}

void Parameter::setType(TypeBaseRef type)
{
	_type = type;
}

} } } // namespace Everbase::InterfaceCompiler::Model
