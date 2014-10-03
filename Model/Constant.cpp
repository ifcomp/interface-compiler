#include "Model/Constant.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Constant::TYPE_NAME = "Constant";

Constant::Constant()
{
}

Constant::~Constant()
{

}

TypeBaseRef Constant::type()
{
    return _type;
}

void Constant::setType(TypeBaseRef type)
{
    _type = type;
}

boost::any Constant::value()
{
    return _value;
}

void Constant::setValue(boost::any value)
{
    _value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
