#include "Model/Class/Constant.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::Constant::TYPE_NAME = "Constant";

Class::Constant::Constant()
{
}

Class::Constant::~Constant()
{

}

ObjectRef Class::Constant::clone() const
{
    ConstantRef newConstant = std::make_shared<Constant>();
    clone(newConstant);
    return newConstant;
}

TypeBaseRef Class::Constant::type() const
{
    return _type;
}

void Class::Constant::setType(TypeBaseRef type)
{
    _type = type;
}

boost::any Class::Constant::value() const
{
    return _value;
}

void Class::Constant::setValue(boost::any value)
{
    _value = value;
}

void Class::Constant::clone(ObjectRef clonedObject) const
{
    using namespace std;

    ConstantRef clonedConstant = dynamic_pointer_cast<Constant>(clonedObject);

    if (clonedConstant)
    {
        Identifiable::clone(clonedConstant);
        clonedConstant->setType(type());
        clonedConstant->setValue(value());
    }
    else
    {
        throw runtime_error("clone() failed: expected Constant - got " + clonedObject->typeName());
    }

}

} } } // namespace Everbase::InterfaceCompiler::Model
