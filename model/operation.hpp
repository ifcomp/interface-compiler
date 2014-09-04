#ifndef OPERATION_H
#define OPERATION_H

#include "model/baseObject.hpp"
#include "model/parameter.hpp"

class Operation : public BaseObject
{
public:
    Operation();

    bool isStatic();
    void setStatic(bool isStatic);

    bool isSynchronous();
    void setSynchronous(bool isSynchronous);

    void addParam(ParameterPtr param);
    std::vector<ParameterPtr> params();

    void setReturnValue(ParameterPtr param);
    ParameterPtr returnValue();

private:
    bool mStaticFunction;
    bool mSynchronous;
    std::vector<ParameterPtr> mParams;
    ParameterPtr mReturnValue;
};

typedef std::shared_ptr<Operation> OperationPtr;

#endif // OPERATION_H
