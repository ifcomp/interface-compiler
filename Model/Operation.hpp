#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Parameter.hpp"

#include <string>
#include <map>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Operation : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "Operation";

private:
    bool mIsStatic;
    bool mIsSynchronous;
    std::map<std::string, ParameterRef> mParams;
    ParameterRef mResult;

public:
    Operation();
    virtual ~Operation();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool isStatic();
    void setStatic(bool isStatic);

    bool isSynchronous();
    void setSynchronous(bool isSynchronous);

    void addParam(ParameterRef param);
    const std::map<std::string, ParameterRef>& params();

    void setResult(ParameterRef result);
    ParameterRef result();
};

typedef std::shared_ptr<Operation> OperationRef;

} } } // namespace Everbase::InterfaceCompiler::Model
