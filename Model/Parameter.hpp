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
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Parameter();
    virtual ~Parameter();

public:
    TypeBaseRef type();
    void setType(TypeBaseRef type);

private:
    TypeBaseRef _type;
};

typedef std::shared_ptr<Parameter> ParameterRef;

} } } // namespace Everbase::InterfaceCompiler::Model
