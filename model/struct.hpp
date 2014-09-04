#ifndef STRUCT_HPP
#define STRUCT_HPP

#include "model/baseObject.hpp"
#include "model/parameter.hpp"

class Struct : public BaseObject
{
public:
    Struct();

    void addParam(ParameterPtr param);
    std::vector<ParameterPtr> params();

private:
    std::vector<ParameterPtr> mParams;

};

#endif // STRUCT_HPP
