#include "Model/Type.hpp"
#include "Model/UnresolvedType.hpp"

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
    auto clonedType = std::make_shared<UnresolvedType>();
    clone(clonedType);
    return clonedType;
}

const ElementRef &Type::primary() const
{
    return _primary;
}

void Type::setPrimary(const ElementRef &primary)
{
    _primary = primary;
}

void Type::addParam(const ElementRef &param)
{
    _params.push_back(param);
}

std::vector<ElementRef> Type::params() const
{
    return _params;
}

void Type::clone(const ObjectRef &clonedObject) const
{
    using namespace std;

    UnresolvedTypeRef unresolvedType = dynamic_pointer_cast<UnresolvedType>(clonedObject);

    if (unresolvedType)
    {
        if (primary())
        {
            unresolvedType->setPrimary(getQualifiedName(primary()));
        }

        for (auto param : params())
        {
            unresolvedType->addParam(getQualifiedName(param));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected UnresolvedType - got " + clonedObject->typeName());
    }
}

std::string Type::getQualifiedName(const ElementRef &element) const
{
    std::string output;

    ObjectRef parent = element->parent();

    while (auto parentElement = std::dynamic_pointer_cast<Identifiable>(parent))
    {
        output = parentElement->longName() + "::" + output;
        parent = parent->parent();
    }

    return output + element->longName();
}

} } } // namespace Everbase::InterfaceCompiler::Model
