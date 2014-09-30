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
    return mapToVector<NamespaceMemberRef>(_members);
}

void Namespace::addMember(NamespaceMemberRef member)
{
    using namespace std;

    string key = member->longName();
    if (_members.find(key) == _members.end())
    {
        _members[key] = member;
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
