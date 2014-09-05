#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include <memory>
#include "model/identifiable.hpp"

namespace Api { namespace Model {

class Value : public Identifiable
{
public:
    Value();
    virtual ~Value();

    int32_t value();
    void setValue(int32_t value);

private:
    int32_t mValue;
};

typedef std::shared_ptr<Value> ValuePtr;

} } // namespace Api::Model

#endif // VALUE_HPP
