#include "Model/UnresolvedType.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

UnresolvedType::UnresolvedType()
{
}

UnresolvedType::~UnresolvedType()
{
}

std::string UnresolvedType::primary()
{
    return mPrimary;
}

void UnresolvedType::setPrimary(std::string typeName)
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

} } } // namespace Everbase::InterfaceCompiler::Model
