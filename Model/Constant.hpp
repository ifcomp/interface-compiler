#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Type.hpp"

#include <boost/any.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Constant : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

private:
    TypeRef _type;
    boost::any _value;

public:
    Constant();
    virtual ~Constant();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypeRef type();
    void setType(TypeRef type);

    boost::any value();
    void setValue(boost::any value);
};

typedef std::shared_ptr<Constant> ConstantRef;

} } } // namespace Everbase::InterfaceCompiler::Model
