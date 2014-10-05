#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Type.hpp"
#include "Model/Class.hpp"

#include <boost/any.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class::Constant : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Constant();
    virtual ~Constant();

public:
    TypeBaseRef type();
    void setType(TypeBaseRef type);

    boost::any value();
    void setValue(boost::any value);

private:
    TypeBaseRef _type;
    boost::any _value;
};

} } } // namespace Everbase::InterfaceCompiler::Model
