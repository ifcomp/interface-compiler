#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include "model/parameter.hpp"

class Observables
{
public:
    Observables();

    void addParam(ParameterPtr param);
    std::vector<ParameterPtr> params();

    void addValue(ParameterPtr value);
    std::vector<ParameterPtr> values();

private:
    std::vector<ParameterPtr> mParams;
    std::vector<ParameterPtr> mValues;
};

#endif // OBSERVABLES_H
