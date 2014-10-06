#include "Model/UnresolvedType.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* UnresolvedType::TYPE_NAME = "UnresolvedType";

UnresolvedType::UnresolvedType()
{
}

UnresolvedType::~UnresolvedType()
{
}

ObjectRef UnresolvedType::clone() const
{
    UnresolvedTypeRef newType = std::make_shared<UnresolvedType>();
    clone(newType);
    return newType;
}

std::string UnresolvedType::primary() const
{
    return _primary;
}

void UnresolvedType::setPrimary(std::string typeName)
{
    _primary = typeName;
}

void UnresolvedType::addParam(std::string param)
{
    _params.push_back(param);
}

const std::vector<std::string> &UnresolvedType::params() const
{
    return _params;
}

void UnresolvedType::clone(ObjectRef clonedObject) const
{
    using namespace std;

    UnresolvedTypeRef clonedType = dynamic_pointer_cast<UnresolvedType>(clonedObject);

    if (clonedType)
    {
        clonedType->setPrimary(primary());

        for (auto param : params())
        {
            clonedType->addParam(param);
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected UnresolvedType - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
