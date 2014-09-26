#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include "model/identifiable.hpp"

namespace Api { namespace Model {

class Value : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "value";

private:
    int32_t mValue;

public:
    Value();
    virtual ~Value();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    int32_t value();
    void setValue(int32_t value);
};

typedef std::shared_ptr<Value> ValuePtr;

} } // namespace Api::Model

#endif // VALUE_HPP
