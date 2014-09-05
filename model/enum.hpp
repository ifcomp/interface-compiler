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
    Enum();
    virtual ~Enum();

    void addValue(ValuePtr value);
    const std::map<std::string, ValuePtr>& values();

private:
    std::map<std::string, ValuePtr> mValues;
};

typedef std::shared_ptr<Enum> EnumPtr;

} } // namespace Api::Model

#endif // ENUM_HPP
