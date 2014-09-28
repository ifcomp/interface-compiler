#pragma once

#include "Model/DomainObject.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class TypeBase : public DomainObject
{
public:
    TypeBase();
    virtual ~TypeBase();
};

typedef std::shared_ptr<TypeBase> TypeBaseRef;

} } } // namespace Everbase::InterfaceCompiler::Model
