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
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Constant();
    virtual ~Constant();
    virtual ObjectRef clone() const override;

public:
    TypeBaseRef type() const;
    void setType(TypeBaseRef type);

    boost::any value() const;
    void setValue(boost::any value);

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    TypeBaseRef _type;
    boost::any _value;
};

} } } // namespace Everbase::InterfaceCompiler::Model
