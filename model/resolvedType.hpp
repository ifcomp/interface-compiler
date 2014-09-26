#ifndef RESOLVEDTYPE_HPP
#define RESOLVEDTYPE_HPP

#include <vector>

#include "model/type.hpp"
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class ResolvedType : public Type
{
public:
    static constexpr auto TYPE_NAME = "type";

private:
    NamespaceMemberPtr mPrimary;
    std::vector<NamespaceMemberPtr> mParams;

public:
    ResolvedType();
    virtual ~ResolvedType();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    NamespaceMemberPtr primary();
    void setPrimary(NamespaceMemberPtr primary);

    void addParam(NamespaceMemberPtr param);
    const std::vector<NamespaceMemberPtr>& params();
};

typedef std::shared_ptr<ResolvedType> ResolvedTypePtr;

} } // namespace Api::Model

#endif // RESOLVEDTYPE_HPP
