#ifndef UNRESOLVEDTYPE_HPP
#define UNRESOLVEDTYPE_HPP

#include <vector>
#include "model/type.hpp"

namespace Api { namespace Model {

class UnresolvedType : public Type
{
public:
    static constexpr auto TYPE_NAME = "unresolved-type";

private:
    std::string mPrimary;
    std::vector<std::string> mParams;

public:
    UnresolvedType();
    virtual ~UnresolvedType();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    std::string primary();
    void setPrimary(std::string primary);

    void addParam(std::string param);
    const std::vector<std::string>& params();
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypePtr;

} } // namespace Api::Model

#endif // UNRESOLVEDTYPE_HPP
