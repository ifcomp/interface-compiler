#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Constant : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "Constant";

private:
    TypeRef mType;
    std::string mValue;

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
