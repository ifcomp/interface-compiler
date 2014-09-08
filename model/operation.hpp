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
    Operation();
    virtual ~Operation();

    bool isStatic();
    void setStatic(bool isStatic);

    bool isSynchronous();
    void setSynchronous(bool isSynchronous);

    void addParam(ParameterPtr param);
    const std::map<std::string, ParameterPtr>& params();

    void setResult(ParameterPtr result);
    ParameterPtr result();

private:
    bool mIsStatic;
    bool mIsSynchronous;
    std::map<std::string, ParameterPtr> mParams;
    ParameterPtr mResult;
};

typedef std::shared_ptr<Operation> OperationPtr;

} } // namespace Api::Model

#endif // OPERATION_HPP
