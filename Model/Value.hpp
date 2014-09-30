#pragma once

#include "Model/Identifiable.hpp"

#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Value : public Identifiable
{
public:
    static const char* TYPE_NAME;

private:
    int32_t _value;

public:
    Value();
    virtual ~Value();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    int32_t value();
    void setValue(int32_t value);
};

typedef std::shared_ptr<Value> ValueRef;

} } } // namespace Everbase::InterfaceCompiler::Model
