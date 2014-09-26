#ifndef DOMAINOBJECT_HPP
#define DOMAINOBJECT_HPP

#include "boost/shared_ptr.hpp"

/**
 * @brief Base class for identifying and linking domain objects.
 * @autor Gunther Lemm <lemm@silpion.de>
 */
class DomainObject;

typedef std::shared_ptr<DomainObject> DomainObjectPtr;

class DomainObject
{
public:
    DomainObject(DomainObjectPtr parentObject = nullptr);

    /**
     * @brief Get pointer to parent object.
     * @return Pointer to parent object
     */
    DomainObjectPtr parentObject();

    /**
     * @brief Set pointer to parent object.
     * @param parentObject Pointer to parent object
     */
    void setParentObject(DomainObjectPtr parentObject);

    /**
     * @brief Get Class identifier
     * @return Indentifier string
     */
    virtual std::string objectTypeName() = 0;

private:
    DomainObjectPtr mParentObject;
};

#endif // DOMAINOBJECT_HPP
