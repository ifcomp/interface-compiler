#include "unresolvedType.hpp"

using namespace Api::Model;

UnresolvedType::UnresolvedType()
{
}

Api::Model::UnresolvedType::~UnresolvedType()
{

}

std::string Api::Model::UnresolvedType::typeName()
{
    return mTypeName;
}

void Api::Model::UnresolvedType::setTypeName(std::string typeName)
{
    mTypeName = typeName;
}
