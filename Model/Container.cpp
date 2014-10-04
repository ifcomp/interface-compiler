#include "Model/Container.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Container::TYPE_NAME = "Container";

const char *Container::containerNames[] =
{
    "Vector",
    "List",
    "Set"
};

Container::Container()
    : _type(ContainerType::UNDEFINED)
{
}


Container::~Container()
{
}


Container::ContainerType Container::type()
{
    return _type;
}


void Container::setType(Container::ContainerType type)
{
    _type = type;
}


std::string Container::typeName()
{
    return containerNames[ int(_type) ];
}


void Container::setType(std::string typeName)
{
    _type = decodeTypeName(typeName);

    if (_type == ContainerType::UNDEFINED)
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

} } } // namespace Everbase::InterfaceCompiler::Model
