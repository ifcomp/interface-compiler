#include <iostream>
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
    using namespace std;

    string key = member->longName();
    if (mMembers.find(key) == mMembers.end())
    {
        mMembers[key] = member;
    }
    else
    {
        cout << "Namespace " << key << " already exists" << endl;
    }
}
