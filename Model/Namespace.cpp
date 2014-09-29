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

std::vector<NamespaceMemberRef> Namespace::members()
{
	std::vector<NamespaceMemberRef> result;

	for( auto m : mMembers )
		result.push_back(m.second);

	return result;
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
