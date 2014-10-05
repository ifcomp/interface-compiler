#include "Model/Object.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Object::Object()
{
}

ObjectRef Object::parent()
{
    return _parent;
}

void Object::setParent(ObjectRef parent)
{
    _parent = parent;
}

} } } // namespace Everbase::InterfaceCompiler::Model
