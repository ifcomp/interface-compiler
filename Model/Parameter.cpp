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
	return mType;
}

void Parameter::setType(TypeBaseRef type)
{
	mType = type;
}

} } } // namespace Everbase::InterfaceCompiler::Model
