#include "model/type.hpp"

using namespace Api::Model;

Type::Type()
{
}

Type::~Type()
{
}

NamespaceMemberPtr Type::primary()
{
	return mPrimary;
}

void Type::setPrimary(NamespaceMemberPtr primary)
{
	mPrimary = primary;
}

void Type::addParam(NamespaceMemberPtr param)
{
	mParams.push_back(param);
}

const std::vector<NamespaceMemberPtr>& Type::params()
{
	return mParams;
}
