#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Namespace::Namespace()
{
}

Namespace::Namespace(std::string longName)
{
    setLongName(longName);
}

Namespace::~Namespace()
{
}

const std::map<std::string, NamespaceMemberRef>& Namespace::members()
{
	return mMembers;
}

void Namespace::addMember(NamespaceMemberRef member)
{
    using namespace std;

    string key = member->longName();
    if (mMembers.find(key) == mMembers.end())
    {
        mMembers[key] = member;
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
