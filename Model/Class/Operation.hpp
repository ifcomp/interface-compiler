#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Parameter.hpp"
#include "Model/Class.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class::Operation : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Operation();
    virtual ~Operation();

public:
    bool isStatic();
    void setStatic(bool isStatic);

    bool isSynchronous();
    void setSynchronous(bool isSynchronous);

    void addParam(ParameterRef param);
    std::vector<ParameterRef> params();

    void setResult(ParameterRef result);
    ParameterRef result();

private:
    bool _isStatic;
    bool _isSynchronous;
    std::vector<ParameterRef> _params;
    ParameterRef _result;
};

} } } // namespace Everbase::InterfaceCompiler::Model
