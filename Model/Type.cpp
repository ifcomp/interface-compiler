#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Type::TYPE_NAME = "Type";

Type::Type()
{
}

Type::~Type()
{
}

ObjectRef Type::clone() const
{
    TypeRef clonedType = std::make_shared<Type>();
    clone(clonedType);
    return clonedType;
}

ElementRef Type::primary() const
{
    return _primary;
}

void Type::setPrimary(ElementRef primary)
{
    _primary = primary;
}

void Type::addParam(ElementRef param)
{
    _params.push_back(param);
}

std::vector<ElementRef> Type::params() const
{
    return _params;
}

void Type::clone(ObjectRef clonedObject) const
{
    using namespace std;

    TypeRef clonedType = dynamic_pointer_cast<Type>(clonedObject);

    if (clonedType)
    {
        clonedType->setPrimary(dynamic_pointer_cast<Element>(primary()->clone()));

        for (auto param : params())
        {
            clonedType->addParam(dynamic_pointer_cast<Element>(param->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Type - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
