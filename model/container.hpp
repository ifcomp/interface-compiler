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

    static std::string listSupportedTypes();
};

typedef std::shared_ptr<Container> ContainerPtr;

} } // namespace Api::Model

#endif // CONTAINER_HPP
