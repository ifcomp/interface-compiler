#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "model/identifiable.hpp"
#include "model/type.hpp"

namespace Api { namespace Model {

class Constant : public NamespaceMember
{
public:
    Constant();
    virtual ~Constant();

    TypePtr type();
    void setType(TypePtr type);

    std::string value();
    void setValue(std::string value);

private:
    TypePtr mType;
    std::string mValue;
};

typedef std::shared_ptr<Constant> ConstantPtr;

} } // namespace Api::Model

#endif // CONSTANT_HPP
