#include "constant.hpp"

using namespace Api::Model;

Constant::Constant()
{
}

Constant::~Constant()
{

}

TypePtr Constant::type()
{
    return mType;
}

void Constant::setType(TypePtr type)
{
    mType = type;
}

std::string Api::Model::Constant::value()
{
    return mValue;
}

void Api::Model::Constant::setValue(std::string value)
{
    mValue = value;
}
