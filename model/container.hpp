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
        VECTOR,
        LIST,
        SET
    };

private:
    ContainerType mType;

public:
    Container();
    virtual ~Container();

    ContainerType type();
    void setType(ContainerType type);
};

typedef std::shared_ptr<Container> ContainerPtr;

} } // namespace Api::Model

#endif // CONTAINER_HPP
