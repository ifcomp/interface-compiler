#include "unresolvedType.hpp"

using namespace Api::Model;

UnresolvedType::UnresolvedType()
{
}

Api::Model::UnresolvedType::~UnresolvedType()
{
}

std::string Api::Model::UnresolvedType::primary()
{
    return mPrimary;
}

void Api::Model::UnresolvedType::setPrimary(std::string typeName)
{
    mPrimary = typeName;
}

void UnresolvedType::addParam(std::string param)
{
    mParams.push_back(param);
}

const std::vector<std::string> &UnresolvedType::params()
{
    return mParams;
}
