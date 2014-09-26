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
    static constexpr auto TYPE_NAME = "namespace";

private:
    std::map<std::string, NamespaceMemberPtr> mMembers;

public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addMember(NamespaceMemberPtr member);
    const std::map<std::string, NamespaceMemberPtr>& members();
};

typedef std::shared_ptr<Namespace> NamespacePtr;

} } // namespace Api::Model

#endif // NAMESPACE_HPP
