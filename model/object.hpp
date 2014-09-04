#ifndef CLASS_H
#define CLASS_H

#include "model/baseObject.hpp"
#include "model/operation.hpp"

class Object : public BaseObject
{
public:
    Object();

    bool isValueType();
    void setValueType(bool isValueType);

    bool isAbstractType();
    void setAbstractType(bool isAbstractType);

    std::shared_ptr<BaseObject> parent();
    void setParent(std::shared_ptr<BaseObject> parent);

    void addOperation(ParameterPtr param);
    std::vector<ParameterPtr> operations();

private:
    bool mValueType;
    bool mAbstractType;
    std::shared_ptr<BaseObject> mParent;
    std::vector<OperationPtr> mOperations;
};

typedef std::shared_ptr<Object> ClassPtr;

#endif // CLASS_H
