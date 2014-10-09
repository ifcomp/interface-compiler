#include "Model/Object.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Object::Object()
{
}

const ObjectRef &Object::parent() const
{
    return _parent;
}

void Object::setParent(const ObjectRef &parent)
{
    _parent = parent;
}

} } } // namespace Everbase::InterfaceCompiler::Model
