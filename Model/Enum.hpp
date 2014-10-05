#pragma once

#include "Model/Element.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Enum : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string objectTypeName() override { return TYPE_NAME; }

public:
	class Value;
	typedef std::shared_ptr<Value> ValueRef;

public:
    Enum();
    virtual ~Enum();

    void addValue(ValueRef value);
    std::vector<ValueRef> values();

private:
    std::vector<ValueRef> _values;
};

typedef std::shared_ptr<Enum> EnumRef;

} } } // namespace Everbase::InterfaceCompiler::Model
