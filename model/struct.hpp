#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <string>
#include <map>
#include <memory>
#include "model/namespace_member.hpp"
#include "model/parameter.hpp"

namespace Api { namespace Model {

class Struct : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "struct";

private:
    std::map<std::string, ParameterPtr> mFields;

public:
    Struct();
    virtual ~Struct();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addField(ParameterPtr field);
    const std::map<std::string, ParameterPtr>& fields();
};

typedef std::shared_ptr<Struct> StructPtr;

} } // namespace Api::Model

#endif // STRUCT_HPP
