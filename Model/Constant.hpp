#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Constant : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

private:
    TypeRef _Type;
    std::string _value;

public:
    Constant();
    virtual ~Constant();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypeRef type();
    void setType(TypeRef type);

    std::string value();
    void setValue(std::string value);
};

typedef std::shared_ptr<Constant> ConstantRef;

} } } // namespace Everbase::InterfaceCompiler::Model
