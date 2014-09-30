#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Type::TYPE_NAME = "Type";

Type::Type()
{
}

Type::~Type()
{
}

NamespaceMemberRef Type::primary()
{
    return _primary;
}

void Type::setPrimary(NamespaceMemberRef primary)
{
    _primary = primary;
}

void Type::addParam(NamespaceMemberRef param)
{
    _params.push_back(param);
}

const std::vector<NamespaceMemberRef>& Type::params()
{
    return _params;
}

} } } // namespace Everbase::InterfaceCompiler::Model
