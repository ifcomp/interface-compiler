#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Type.hpp"

#include <boost/any.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Constant : public Identifiable
{
public:
    static const char* TYPE_NAME;

private:
    TypeBaseRef _type;
    boost::any _value;

public:
    Constant();
    virtual ~Constant();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypeBaseRef type();
    void setType(TypeBaseRef type);

    boost::any value();
    void setValue(boost::any value);
};

typedef std::shared_ptr<Constant> ConstantRef;

} } } // namespace Everbase::InterfaceCompiler::Model
