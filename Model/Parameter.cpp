#include "Model/Parameter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Parameter::TYPE_NAME = "Parameter";

Parameter::Parameter()
{
}

Parameter::~Parameter()
{
}

ObjectRef Parameter::clone() const
{
    ParameterRef newParam = std::make_shared<Parameter>();
    clone(newParam);
    return newParam;
}

TypeBaseRef Parameter::type() const
{
	return _type;
}

void Parameter::setType(TypeBaseRef type)
{
    _type = type;
}

void Parameter::clone(ObjectRef clonedObject) const
{
    ParameterRef clonedParam = std::dynamic_pointer_cast<Parameter>(clonedObject);

    if (clonedParam)
    {
        Identifiable::clone(clonedParam);
        clonedParam->setType(type());
    }
    else
    {
        throw std::runtime_error("clone() failed: expected Parameter - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
