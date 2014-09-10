#ifndef RESOLVEDTYPE_HPP
#define RESOLVEDTYPE_HPP

#include "model/type.hpp"

namespace Api { namespace Model {

class ResolvedType : public Type
{
public:
    ResolvedType();
    virtual ~ResolvedType();

    NamespaceMemberPtr primary();
    void setPrimary(NamespaceMemberPtr primary);

    void addParam(NamespaceMemberPtr param);
    const std::vector<NamespaceMemberPtr>& params();

private:
    NamespaceMemberPtr mPrimary;
    std::vector<NamespaceMemberPtr> mParams;
};

typedef std::shared_ptr<ResolvedType> ResolvedTypePtr;

} } // namespace Api::Model

#endif // RESOLVEDTYPE_HPP
