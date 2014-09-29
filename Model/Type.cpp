#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

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
