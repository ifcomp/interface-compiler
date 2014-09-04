#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "model/baseObject.hpp"

class Container : public BaseObject
{
public:
    enum ContainerType
    {
        VECTOR,
        LIST,
        SET
    };

private:
    ContainerType mType;
    BaseObjectPtr mPayloadType;
    std::string mMapTo;

public:
    Container();

    ContainerType type();
    void setType(ContainerType type);

    BaseObjectPtr payloadType();
    void setPayloadType(BaseObjectPtr type);

    std::string mapTo();
    void setMapTo(std::string mapTo);
};

#endif // CONTAINER_HPP
