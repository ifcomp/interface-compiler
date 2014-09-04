#ifndef PARAMETERTYPE_H
#define PARAMETERTYPE_H

#include <string>
#include <boost/shared_ptr.hpp>

class ParameterType
{
public:
    ParameterType();

    std::string typeNamespace();
    void setContext(std::string context);

    std::string typeName();
    void setTypeName(std::string typeName);

    std::shared_ptr<ParameterType> subType();
    void setSubType(std::shared_ptr<ParameterType> subType);

private:
    std::string mTypeNamespace;
    std::string mTypeName;
    std::shared_ptr<ParameterType> mSubType;
};

#endif // PARAMETERTYPE_H
