#pragma once

#include "Model/Identifiable.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class NamespaceMember : public Identifiable
{
public:
    NamespaceMember();
    virtual ~NamespaceMember();
};

typedef std::shared_ptr<NamespaceMember> NamespaceMemberRef;

} } } // namespace Everbase::InterfaceCompiler::Model
