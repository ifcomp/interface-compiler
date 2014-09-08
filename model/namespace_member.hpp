#ifndef NAMESPACE_MEMBER_HPP
#define NAMESPACE_MEMBER_HPP

#include <string>
#include <memory>
#include "model/identifiable.hpp"

namespace Api { namespace Model {

class NamespaceMember : public Identifiable
{
public:
    NamespaceMember();
    virtual ~NamespaceMember();
};

typedef std::shared_ptr<NamespaceMember> NamespaceMemberPtr;

} } // namespace Api::Model

#endif // NAMESPACE_MEMBER_HPP
