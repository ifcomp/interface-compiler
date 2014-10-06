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
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Struct();
    virtual ~Struct();
    virtual ObjectRef clone() const override;

public:
    void addField(ParameterRef field);
    std::vector<ParameterRef> fields() const;

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    std::vector<ParameterRef> _fields;
};

typedef std::shared_ptr<Struct> StructRef;

} } } // namespace Everbase::InterfaceCompiler::Model
