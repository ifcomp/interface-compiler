#pragma once

#include "Model/TypeBase.hpp"
#include "Model/Element.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Type : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Type();
    virtual ~Type();
    virtual ObjectRef clone() const override;

public:
    ElementRef primary() const;
    void setPrimary(ElementRef primary);

    void addParam(ElementRef param);
    std::vector<ElementRef> params() const;

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    ElementRef _primary;
    std::vector<ElementRef> _params;
};

typedef std::shared_ptr<Type> TypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
