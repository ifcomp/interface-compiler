#include "primitive.hpp"

using namespace Api::Model;

const char *Primitive::primitiveNames[] =
{
    "byte",
    "uint32",
    "uint64",
    "float",
    "bool",
    "timestamp",
    "string",
    "uuid",
    "buffer",
    "const-buffer"
};


Primitive::Primitive()
    : mType(PrimitiveType::UNDEFINED)
{
}


Primitive::~Primitive()
{
}


Primitive::PrimitiveType Primitive::type()
{
	return mType;
}


void Primitive::setType(Primitive::PrimitiveType type)
{
    mType = type;
}


void Primitive::setType(std::string typeName)
{
    mType = decodeTypeName(typeName);

    if (mType == PrimitiveType::UNDEFINED)
    {
        throw std::runtime_error("unknown primitive type " + typeName + "\n");
    }
}


Primitive::PrimitiveType Primitive::decodeTypeName(std::string typeName)
{
    for (int n = 0; n < int(PrimitiveType::_PRIMITIVE_COUNT_); ++n)
    {
        if (typeName == primitiveNames[n])
        {
            return (PrimitiveType) n;
        }
    }
    return PrimitiveType::UNDEFINED;
}


std::string Primitive::listSupportedTypes()
{
    std::string list = "the following primitive types are supported:\n";

    for (int n = 0; n < int(PrimitiveType::_PRIMITIVE_COUNT_); ++n)
    {
        list += "- " + std::string(primitiveNames[n]) + "\n";
    }
    return list;
}
