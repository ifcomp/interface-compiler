#include "Model/Primitive.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Primitive::primitiveNames[] =
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

const char* Primitive::TYPE_NAME = "Primitive";

Primitive::Primitive()
    : _type(PrimitiveType::UNDEFINED)
{
}


Primitive::~Primitive()
{
}


Primitive::PrimitiveType Primitive::type()
{
	return _type;
}


void Primitive::setType(Primitive::PrimitiveType type)
{
    _type = type;
}


std::string Primitive::typeName()
{
    return primitiveNames[ int(_type) ];
}


void Primitive::setType(std::string typeName)
{
    _type = decodeTypeName(typeName);

    if (_type == PrimitiveType::UNDEFINED)
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

} } } // namespace Everbase::InterfaceCompiler::Model
