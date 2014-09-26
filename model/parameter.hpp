#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>
#include <memory>
#include "model/identifiable.hpp"
#include "model/type.hpp"

namespace Api { namespace Model {

class Parameter : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "parameter";

private:
    TypePtr mType;

public:
    Parameter();
    virtual ~Parameter();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypePtr type();
    void setType(TypePtr type);
};

typedef std::shared_ptr<Parameter> ParameterPtr;

} } // namespace Api::Model

#endif // PARAMETER_HPP
