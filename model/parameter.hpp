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
    Parameter();
    virtual ~Parameter();

    TypePtr type();
    void setType(TypePtr type);

private:
    TypePtr mType;
};

typedef std::shared_ptr<Parameter> ParameterPtr;

} } // namespace Api::Model

#endif // PARAMETER_HPP
