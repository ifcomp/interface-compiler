#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "model/baseObject.hpp"

class Parameter
{
public:
    Parameter();

    std::string name();
    void setName(std::string name);

    BaseObjectPtr type();
    void setType();

private:
    std::string mName;
    BaseObjectPtr mType;
};

typedef std::shared_ptr<Parameter> ParameterPtr;

#endif // PARAMETER_H
