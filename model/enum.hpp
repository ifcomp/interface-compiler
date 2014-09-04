#ifndef ENUM_HPP
#define ENUM_HPP

#include "model/baseObject.hpp"
#include "model/value.hpp"

class Enum : public BaseObject
{
public:
    Enum();

    void addValue(Value value);
    std::vector<ValuePtr> values();

private:
    std::vector<ValuePtr> mValues;
};

#endif // ENUM_HPP
