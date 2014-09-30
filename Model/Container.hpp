#pragma once

#include "Model/NamespaceMember.hpp"

#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Container : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

    enum class ContainerType
    {
        UNDEFINED = -1,
        VECTOR,
        LIST,
        SET,
        _CONTAINER_COUNT_               ///< Number of enum entries. THIS MUST BE THE LAST ENTRY!
    };

    /**
     * @brief Mapping of primitive name strings to PrimitiveType enum
     */
    static const char *containerNames[int(ContainerType::_CONTAINER_COUNT_)];

private:
    ContainerType _type;

public:
    Container();
    virtual ~Container();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    /**
     * @brief Get ContainerType
     * @return Current ContainerType
     */
    ContainerType type();

    /**
     * @brief Set type by ContainerType.
     * @see ContainerType
     * @param type ContainerType to set
     */
    void setType(ContainerType type);

    /**
     * @brief Get type name
     * @return Type name
     */
    std::string typeName();

    /**
     * @brief Set type by type name.
     * @see containerNames
     * @param typeName Type name as listed in containerNames
     * @throw std::runtime_error if name was not found
     */
    void setType(std::string typeName);

    /**
     * @brief Resolve type name to ContainerType
     * @param typeName Type name as listed in containerNames
     * @return Returns ContainerType (ContainerType::UNDEFINED if name was not found)
     */
    static ContainerType decodeTypeName(std::string typeName);

    /**
     * @brief Return human-readable list of known container types.
     * @return String containing a list of type names, each one in a row.
     */
    static std::string listSupportedTypes();
};

typedef std::shared_ptr<Container> ContainerRef;

} } } // namespace Everbase::InterfaceCompiler::Model
