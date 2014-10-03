#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Value.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Enum : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

private:
    std::vector<ValueRef> _values;

public:
    Enum();
    virtual ~Enum();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addValue(ValueRef value);
    std::vector<ValueRef> values();
};

typedef std::shared_ptr<Enum> EnumRef;

} } } // namespace Everbase::InterfaceCompiler::Model
