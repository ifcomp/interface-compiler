#include "Model/Primitive.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Primitive::TYPE_NAME = "Primitive";

const char* Primitive::underlyingNames[] =
{
    "Byte",
    "UInt16",
    "UInt32",
    "UInt64",
    "Boolean",
    "Timestamp",
    "String",
    "Uuid",
    "Buffer",
    "ConstBuffer",
    "Vector",
    "List",
    "Set",
    "Map"
};

Primitive::Primitive()
    : _underlying(Underlying::UNDEFINED)
{
}

Primitive::~Primitive()
{
}

Primitive::Underlying Primitive::underlying()
{
	return _underlying;
}

void Primitive::setUnderlying(Primitive::Underlying underlying)
{
    _underlying = underlying;
}

std::string Primitive::underlyingName()
{
    return underlyingNames[ int(_underlying) ];
}

void Primitive::setUnderlying(std::string underlyingName)
{
    _underlying = decodeUnderlyingName(underlyingName);

    if (_underlying == Underlying::UNDEFINED)
    {
        throw std::runtime_error("unknown primitive underlying " + underlyingName + "\n");
    }
}

Primitive::Underlying Primitive::decodeUnderlyingName(std::string underlyingName)
{
    for (int n = 0; n < int(Underlying::_COUNT_); ++n)
    {
        if (underlyingName == underlyingNames[n])
        {
            return (Underlying) n;
        }
    }
    return Underlying::UNDEFINED;
}

std::string Primitive::listSupportedUnderlying()
{
    std::string list = "the following primitive underlyings are supported:\n";

    for (int n = 0; n < int(Underlying::_COUNT_); ++n)
    {
        list += "- " + std::string(underlyingNames[n]) + "\n";
    }
    return list;
}

} } } // namespace Everbase::InterfaceCompiler::Model
