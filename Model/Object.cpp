#include "Model/Object.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Object::Object()
{
}

ObjectRef Object::parentObject()
{
    return _parentObject;
}

void Object::setParentObject(ObjectRef parentObject)
{
    _parentObject = parentObject;
}

} } } // namespace Everbase::InterfaceCompiler::Model
