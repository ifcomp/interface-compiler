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
    Struct();
    virtual ~Struct();

    void addField(ParameterPtr field);
    const std::map<std::string, ParameterPtr>& fields();

private:
    std::map<std::string, ParameterPtr> mFields;
};

typedef std::shared_ptr<Struct> StructPtr;

} } // namespace Api::Model

#endif // STRUCT_HPP
