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
    return mType;
}

void Constant::setType(TypeRef type)
{
    mType = type;
}

std::string Constant::value()
{
    return mValue;
}

void Constant::setValue(std::string value)
{
    mValue = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
