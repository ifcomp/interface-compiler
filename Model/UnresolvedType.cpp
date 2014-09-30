#include "Model/UnresolvedType.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* UnresolvedType::TYPE_NAME = "UnresolvedType";

UnresolvedType::UnresolvedType()
{
}

UnresolvedType::~UnresolvedType()
{
}

std::string UnresolvedType::primary()
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

const std::vector<std::string> &UnresolvedType::params()
{
    return _params;
}

} } } // namespace Everbase::InterfaceCompiler::Model
