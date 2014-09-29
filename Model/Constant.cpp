#include "Model/Constant.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Constant::Constant()
{
}

Constant::~Constant()
{

}

TypeRef Constant::type()
{
    return _Type;
}

void Constant::setType(TypeRef type)
{
    _Type = type;
}

std::string Constant::value()
{
    return _value;
}

void Constant::setValue(std::string value)
{
    _value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
