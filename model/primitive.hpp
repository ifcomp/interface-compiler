#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <memory>
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class Primitive : public NamespaceMember
{
public:
    enum class PrimitiveType
    {
        BYTE,
        UINT32,
        UINT64,
        FLOAT,
        BOOLEAN,
        TIMESTAMP,
        STRING,
        UUID,
        BUFFER,
        CONST_BUFFER
    };

private:
    PrimitiveType mType;

public:
    Primitive();
    virtual ~Primitive();

    PrimitiveType type();
    void setType(PrimitiveType type);
};

typedef std::shared_ptr<Primitive> PrimitivePtr;

} } // namespace Api::Model

#endif // PRIMITIVE_HPP
