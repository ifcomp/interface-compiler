#pragma once

#include "Model/Object.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class TypeBase : public Object
{
public:
    TypeBase();
    virtual ~TypeBase();
};

typedef std::shared_ptr<TypeBase> TypeBaseRef;

} } } // namespace Everbase::InterfaceCompiler::Model
