#pragma once

#include "Model/Element.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Enum : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
	class Value;
	typedef std::shared_ptr<Value> ValueRef;

public:
    Enum();
    virtual ~Enum();
    virtual ObjectRef clone() const override;

public:
    void addValue(ValueRef value);
    std::vector<ValueRef> values() const;

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    std::vector<ValueRef> _values;
};

typedef std::shared_ptr<Enum> EnumRef;

} } } // namespace Everbase::InterfaceCompiler::Model
