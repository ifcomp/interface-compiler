#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Type.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Parameter : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "Parameter";

private:
    TypeBaseRef _type;

public:
    Parameter();
    virtual ~Parameter();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypeBaseRef type();
    void setType(TypeBaseRef type);
};

typedef std::shared_ptr<Parameter> ParameterRef;

} } } // namespace Everbase::InterfaceCompiler::Model
