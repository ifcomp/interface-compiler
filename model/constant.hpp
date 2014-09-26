#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "model/namespace_member.hpp"
#include "model/type.hpp"

namespace Api { namespace Model {

class Constant : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "constant";

private:
    TypePtr mType;
    std::string mValue;

public:
    Constant();
    virtual ~Constant();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    TypePtr type();
    void setType(TypePtr type);

    std::string value();
    void setValue(std::string value);
};

typedef std::shared_ptr<Constant> ConstantPtr;

} } // namespace Api::Model

#endif // CONSTANT_HPP
