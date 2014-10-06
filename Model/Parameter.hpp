#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Type.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Parameter : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Parameter();
    virtual ~Parameter();
    virtual ObjectRef clone() const override;

public:
    TypeBaseRef type() const;
    void setType(TypeBaseRef type);

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    TypeBaseRef _type;
};

typedef std::shared_ptr<Parameter> ParameterRef;

} } } // namespace Everbase::InterfaceCompiler::Model
