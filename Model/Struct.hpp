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
    static const char* TYPE_NAME;

private:
    std::map<std::string, ParameterRef> _fields;

public:
    Struct();
    virtual ~Struct();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addField(ParameterRef field);
    std::vector<ParameterRef> fields();
};

typedef std::shared_ptr<Struct> StructRef;

} } } // namespace Everbase::InterfaceCompiler::Model
