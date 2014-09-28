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
    return mPrimary;
}

void Type::setPrimary(NamespaceMemberRef primary)
{
    mPrimary = primary;
}

void Type::addParam(NamespaceMemberRef param)
{
    mParams.push_back(param);
}

const std::vector<NamespaceMemberRef>& Type::params()
{
    return mParams;
}

} } } // namespace Everbase::InterfaceCompiler::Model
