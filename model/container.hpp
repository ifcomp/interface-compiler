#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <memory>
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class Container : public NamespaceMember
{
public:
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
    ContainerType mType;

public:
    Container();
    virtual ~Container();

    ContainerType type();
    void setType(ContainerType type);

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
    ContainerType resolveType(std::string typeName);

    /**
     * @brief Return human-readable list of known container types.
     * @return String containing a list of type names, each one in a row.
     */
    static std::string listSupportedTypes();
};

typedef std::shared_ptr<Container> ContainerPtr;

} } // namespace Api::Model

#endif // CONTAINER_HPP
