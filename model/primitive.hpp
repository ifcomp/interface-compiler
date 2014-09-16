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
        UNDEFINED = -1,
        BYTE,
        UINT32,
        UINT64,
        FLOAT,
        BOOLEAN,
        TIMESTAMP,
        STRING,
        UUID,
        BUFFER,
        CONST_BUFFER,
        _PRIMITIVE_COUNT_       ///< Number of enum entries. THIS MUST BE THE LAST ENTRY!
    };

    /**
     * @brief Mapping of primitive name strings to PrimitiveType enum
     */
    static const char *primitiveNames[int(PrimitiveType::_PRIMITIVE_COUNT_)];

private:
    PrimitiveType mType;

public:
    Primitive();
    virtual ~Primitive();

    PrimitiveType type();
    void setType(PrimitiveType type);

    static std::string listSupportedTypes();
};

typedef std::shared_ptr<Primitive> PrimitivePtr;

} } // namespace Api::Model

#endif // PRIMITIVE_HPP
