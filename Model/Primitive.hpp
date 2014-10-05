#pragma once

#include "Model/NamespaceMember.hpp"

#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Primitive : public NamespaceMember
{
public:

    static const char* TYPE_NAME;

    enum class PrimitiveType
    {
        UNDEFINED = -1,
        BYTE,
        UINT16,
        UINT32,
        UINT64,
        BOOLEAN,
        TIMESTAMP,
        STRING,
        UUID,
        BUFFER,
        CONST_BUFFER,
        VECTOR,
        LIST,
        SET,
        _PRIMITIVE_COUNT_       ///< Number of enum entries. THIS MUST BE THE LAST ENTRY!
    };

    /**
     * @brief Mapping of primitive name strings to PrimitiveType enum
     */
    static const char *primitiveNames[int(PrimitiveType::_PRIMITIVE_COUNT_)];

private:
    PrimitiveType _type;

public:
    Primitive();
    virtual ~Primitive();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

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
     * @brief Get type name
     * @return Type name
     */
    std::string typeName();

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

typedef std::shared_ptr<Primitive> PrimitiveRef;

} } } // namespace Everbase::InterfaceCompiler::Model
