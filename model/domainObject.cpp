#include "domainObject.hpp"

DomainObject::DomainObject(DomainObjectPtr parentObject)
{
}

DomainObjectPtr DomainObject::parentObject()
{
    return mParentObject;
}

void DomainObject::setParentObject(DomainObjectPtr parentObject)
{
    mParentObject = parentObject;
}
