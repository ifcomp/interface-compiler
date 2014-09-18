#include "container.hpp"

using namespace Api::Model;

const char *Container::containerNames[] =
{
    "vector",
    "list",
    "set"
};


Container::Container()
    : mType(ContainerType::UNDEFINED)
{
}


Container::~Container()
{
}


Container::ContainerType Container::type()
{
	return mType;
}


void Container::setType(Container::ContainerType type)
{
    mType = type;
}


std::string Container::typeName()
{
    return containerNames[ int(mType) ];
}


void Container::setType(std::string typeName)
{
    mType = decodeTypeName(typeName);

    if (mType == ContainerType::UNDEFINED)
    {
        throw std::runtime_error("unknown container type " + typeName + "\n");
    }
}


Container::ContainerType Container::decodeTypeName(std::string typeName)
{
    for (int n = 0; n < int(ContainerType::_CONTAINER_COUNT_); ++n)
    {
        if (typeName == containerNames[n])
        {
            return (ContainerType) n;
        }
    }
    return ContainerType::UNDEFINED;
}


std::string Container::listSupportedTypes()
{
    std::string list = "the following container types are supported:\n";

    for (int n = 0; n < int(ContainerType::_CONTAINER_COUNT_); ++n)
    {
        list += "- " + std::string(containerNames[n]) + "\n";
    }
    return list;
}
