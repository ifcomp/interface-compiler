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

std::string Container::listSupportedTypes()
{
    std::string list = "the following container types are supported:\n";

    for (int n = 0; n < int(ContainerType::_CONTAINER_COUNT_); ++n)
    {
        list += "- " + std::string(containerNames[n]) + "\n";
    }
    return list;
}
