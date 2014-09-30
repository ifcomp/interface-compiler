#pragma once

#include "Model/NamespaceMember.hpp"

#include <string>
#include <map>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Namespace : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

private:
    std::map<std::string, NamespaceMemberRef> _members;

public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addMember(NamespaceMemberRef member);
    std::vector<NamespaceMemberRef> members();
};

typedef std::shared_ptr<Namespace> NamespaceRef;

} } } // namespace Everbase::InterfaceCompiler::Model
