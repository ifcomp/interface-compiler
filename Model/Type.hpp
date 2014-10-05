#pragma once

#include "Model/TypeBase.hpp"
#include "Model/Element.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Type : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string objectTypeName() override { return TYPE_NAME; }

private:
    ElementRef _primary;
    std::vector<ElementRef> _params;

public:
    Type();
    virtual ~Type();

    ElementRef primary();
    void setPrimary(ElementRef primary);

    void addParam(ElementRef param);
    const std::vector<ElementRef>& params();
};

typedef std::shared_ptr<Type> TypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
