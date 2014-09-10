#include "resolvedType.hpp"

using namespace Api::Model;

ResolvedType::ResolvedType()
{
}

ResolvedType::~ResolvedType()
{
}

NamespaceMemberPtr ResolvedType::primary()
{
    return mPrimary;
}

void ResolvedType::setPrimary(NamespaceMemberPtr primary)
{
    mPrimary = primary;
}

void ResolvedType::addParam(NamespaceMemberPtr param)
{
    mParams.push_back(param);
}

const std::vector<NamespaceMemberPtr>& ResolvedType::params()
{
    return mParams;
}
