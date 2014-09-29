#include "Model/DomainObject.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

DomainObject::DomainObject(DomainObjectRef parentObject)
{
}

DomainObjectRef DomainObject::parentObject()
{
    return _parentObject;
}

void DomainObject::setParentObject(DomainObjectRef parentObject)
{
    _parentObject = parentObject;
}

} } } // namespace Everbase::InterfaceCompiler::Model
