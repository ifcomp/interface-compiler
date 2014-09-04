#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include <boost/shared_ptr.hpp>

class Value
{
public:
    Value();

    std::string name();
    void setName(std::string name);

    int32_t value();
    void setValue(int32_t value);

private:
    std::string mName;
    int32_t mValue;
};

typedef std::shared_ptr<Value> ValuePtr;

#endif // VALUE_HPP
