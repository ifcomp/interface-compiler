#ifndef ENUM_HPP
#define ENUM_HPP

#include <string>
#include <map>
#include <memory>
#include "model/namespace_member.hpp"
#include "model/value.hpp"

namespace Api { namespace Model {

class Enum : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "enum";

private:
    std::map<std::string, ValuePtr> mValues;

public:
    Enum();
    virtual ~Enum();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addValue(ValuePtr value);
    const std::map<std::string, ValuePtr>& values();
};

typedef std::shared_ptr<Enum> EnumPtr;

} } // namespace Api::Model

#endif // ENUM_HPP
