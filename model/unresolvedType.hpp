#ifndef UNRESOLVEDTYPE_HPP
#define UNRESOLVEDTYPE_HPP

#include "model/type.hpp"

namespace Api { namespace Model {

class UnresolvedType : public Type
{
public:
    UnresolvedType();
    virtual ~UnresolvedType();

    std::string primary();
    void setPrimary(std::string primary);

    void addParam(std::string param);
    const std::vector<std::string>& params();

private:
    std::string mPrimary;
    std::vector<std::string> mParams;
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypePtr;

} } // namespace Api::Model

#endif // UNRESOLVEDTYPE_HPP
