#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>
#include <map>
#include <memory>
#include "model/identifiable.hpp"
#include "model/parameter.hpp"

namespace Api { namespace Model {

class Operation : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "operation";

private:
    bool mIsStatic;
    bool mIsSynchronous;
    std::map<std::string, ParameterPtr> mParams;
    ParameterPtr mResult;

public:
    Operation();
    virtual ~Operation();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool isStatic();
    void setStatic(bool isStatic);

    bool isSynchronous();
    void setSynchronous(bool isSynchronous);

    void addParam(ParameterPtr param);
    const std::map<std::string, ParameterPtr>& params();

    void setResult(ParameterPtr result);
    ParameterPtr result();
};

typedef std::shared_ptr<Operation> OperationPtr;

} } // namespace Api::Model

#endif // OPERATION_HPP
