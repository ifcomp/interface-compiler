#ifndef UNRESOLVEDTYPE_HPP
#define UNRESOLVEDTYPE_HPP

#include "model/type.hpp"

namespace Api { namespace Model {

class UnresolvedType : public Type
{
public:
    UnresolvedType();
    virtual ~UnresolvedType();

    std::string typeName();
    void setTypeName(std::string typeName);

private:
    std::string mTypeName;
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypePtr;

} } // namespace Api::Model

#endif // UNRESOLVEDTYPE_HPP
