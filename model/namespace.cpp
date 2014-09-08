#include "model/namespace.hpp"

using namespace Api::Model;

Namespace::Namespace()
{
}

Namespace::~Namespace()
{
}

const std::map<std::string, NamespaceMemberPtr>& Namespace::members()
{
	return mMembers;
}

void Namespace::addMember(NamespaceMemberPtr member)
{
    mMembers[member->longName()] = member;
}
