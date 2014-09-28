#include "Model/DomainObject.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

DomainObject::DomainObject(DomainObjectRef parentObject)
{
}

DomainObjectRef DomainObject::parentObject()
{
    return mParentObject;
}

void DomainObject::setParentObject(DomainObjectRef parentObject)
{
    mParentObject = parentObject;
}

} } } // namespace Everbase::InterfaceCompiler::Model
