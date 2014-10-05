#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Enum.hpp"

#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Enum::Value : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Value();
    virtual ~Value();

public:
    int32_t value();
    void setValue(int32_t value);

private:
    int32_t _value;
};

} } } // namespace Everbase::InterfaceCompiler::Model
