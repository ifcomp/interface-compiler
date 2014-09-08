#ifndef TYPE_HPP
#define TYPE_HPP

#include <memory>
#include <vector>
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class Type
{
public:
    Type();
    virtual ~Type();

    NamespaceMemberPtr primary();
    void setPrimary(NamespaceMemberPtr primary);

    void addParam(NamespaceMemberPtr param);
    const std::vector<NamespaceMemberPtr>& params();

private:
    NamespaceMemberPtr mPrimary;
    std::vector<NamespaceMemberPtr> mParams;
};

typedef std::shared_ptr<Type> TypePtr;

} } // namespace Api::Model

#endif // TYPE_HPP
