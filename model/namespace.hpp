#ifndef NAMESPACE_HPP
#define NAMESPACE_HPP

#include <string>
#include <map>
#include <memory>
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class Namespace : public NamespaceMember
{
public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();

    void addMember(NamespaceMemberPtr member);
    const std::map<std::string, NamespaceMemberPtr>& members();

private:
    std::map<std::string, NamespaceMemberPtr> mMembers;
};

typedef std::shared_ptr<Namespace> NamespacePtr;

} } // namespace Api::Model

#endif // NAMESPACE_HPP
