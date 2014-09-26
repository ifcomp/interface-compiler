#ifndef TYPE_HPP
#define TYPE_HPP

#include "model/domainObject.hpp"

namespace Api { namespace Model {

class Type : public DomainObject
{
public:
    Type();
    virtual ~Type();
};

typedef std::shared_ptr<Type> TypePtr;

} } // namespace Api::Model

#endif // TYPE_HPP
