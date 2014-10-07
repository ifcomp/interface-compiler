#include "Model/Object.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Object::Object()
{
}

ObjectRef Object::parent() const
{
    return _parent;
}

void Object::setParent(ObjectRef parent)
{
    _parent = parent;
}

} } } // namespace Everbase::InterfaceCompiler::Model
