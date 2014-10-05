#include "Model/Class/Constant.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::Constant::TYPE_NAME = "Constant";

Class::Constant::Constant()
{
}

Class::Constant::~Constant()
{

}

TypeBaseRef Class::Constant::type()
{
    return _type;
}

void Class::Constant::setType(TypeBaseRef type)
{
    _type = type;
}

boost::any Class::Constant::value()
{
    return _value;
}

void Class::Constant::setValue(boost::any value)
{
    _value = value;
}

} } } // namespace Everbase::InterfaceCompiler::Model
