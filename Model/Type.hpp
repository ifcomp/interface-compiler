#pragma once

#include "Model/TypeBase.hpp"
#include "Model/Element.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Type : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Type();
    virtual ~Type();

public:
    ElementRef primary();
    void setPrimary(ElementRef primary);

    void addParam(ElementRef param);
    const std::vector<ElementRef>& params();

private:
    ElementRef _primary;
    std::vector<ElementRef> _params;
};

typedef std::shared_ptr<Type> TypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
