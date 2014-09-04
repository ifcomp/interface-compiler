#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "model/parametertype.hpp"

class Parameter
{
public:
    Parameter();

    std::string name();
    void setName(std::string name);

    ParameterType type();
    void setType();

private:
    ParameterType mType;
};

typedef std::shared_ptr<Parameter> ParameterPtr;

#endif // PARAMETER_H
