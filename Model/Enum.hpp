#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Value.hpp"

#include <string>
#include <map>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Enum : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "Enum";

private:
    std::map<std::string, ValueRef> _values;

public:
    Enum();
    virtual ~Enum();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addValue(ValueRef value);
    std::vector<ValueRef> values();
};

typedef std::shared_ptr<Enum> EnumRef;

} } } // namespace Everbase::InterfaceCompiler::Model
