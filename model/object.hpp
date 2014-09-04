#ifndef CLASS_H
#define CLASS_H

#include "model/object.hpp"
#include "model/operation.hpp"

class Class : public Object
{
public:
    Class();

    bool isValueType();
    void setValueType(bool isValueType);

    bool isAbstractType();
    void setAbstractType(bool isAbstractType);

    void addOperation(ParameterPtr param);
    std::vector<ParameterPtr> operations();

private:
    bool mValueType;
    bool mAbstractType;
    std::vector<OperationPtr> mOperations;
};

typedef std::shared_ptr<Class> ClassPtr;

#endif // CLASS_H
