#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Namespace::TYPE_NAME = "Namespace";

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
    return _members;
}

void Namespace::addMember(NamespaceMemberRef member)
{
	_members.push_back(member);
}

} } } // namespace Everbase::InterfaceCompiler::Model
