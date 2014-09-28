#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Parameter.hpp"

#include <string>
#include <map>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Struct : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "Struct";

private:
    std::map<std::string, ParameterRef> mFields;

public:
    Struct();
    virtual ~Struct();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addField(ParameterRef field);
    const std::map<std::string, ParameterRef>& fields();
};

typedef std::shared_ptr<Struct> StructRef;

} } } // namespace Everbase::InterfaceCompiler::Model
