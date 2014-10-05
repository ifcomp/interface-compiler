#pragma once

#include "Model/Element.hpp"
#include "Model/Parameter.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Struct : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Struct();
    virtual ~Struct();

public:
    void addField(ParameterRef field);
    std::vector<ParameterRef> fields();

private:
    std::vector<ParameterRef> _fields;
};

typedef std::shared_ptr<Struct> StructRef;

} } } // namespace Everbase::InterfaceCompiler::Model
