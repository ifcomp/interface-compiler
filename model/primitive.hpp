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

    /**
     * @brief Get PrimitiveType
     * @return Current PrimitiveType
     */
    PrimitiveType type();

    /**
     * @brief Set type by PrimitiveType.
     * @see PrimitiveType
     * @param type PrimitiveType to set
     */
    void setType(PrimitiveType type);

    /**
     * @brief Set type by type name.
     * @see primitiveNames
     * @param typeName Type name as listed in primitiveNames
     * @throw std::runtime_error if name was not found
     */
    void setType(std::string typeName);

    /**
     * @brief Decode type name to PrimitiveType
     * @param typeName Type name as listed in primitiveNames
     * @return Returns PrimitiveType (PrimitiveType::UNDEFINED if name was not found)
     */
    static PrimitiveType decodeTypeName(std::string typeName);

    /**
     * @brief Return human-readable list of known primitive types.
     * @return String containing a list of type names, each one in a row.
     */
    static std::string listSupportedTypes();
};

typedef std::shared_ptr<Primitive> PrimitivePtr;

} } // namespace Api::Model

#endif // PRIMITIVE_HPP
