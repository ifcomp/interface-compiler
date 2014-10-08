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
//    "Buffer",
//    "ConstBuffer",
    "Vector",
    "List",
    "Set",
    "Map"
};

const unsigned int Primitive::underlyingParamCounts[] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
//    0,
//    0,
    1,
    1,
    1,
    2
};

Primitive::Primitive()
    : _underlying(Underlying::UNDEFINED)
{
}

Primitive::~Primitive()
{
}

ObjectRef Primitive::clone() const
{
    PrimitiveRef newPrimitive = std::make_shared<Primitive>();
    clone(newPrimitive);
    return newPrimitive;
}

Primitive::Underlying Primitive::underlying() const
{
	return _underlying;
}

void Primitive::setUnderlying(Primitive::Underlying underlying)
{
    _underlying = underlying;
}

std::string Primitive::underlyingName() const
{
    return underlyingNames[ int(_underlying) ];
}

unsigned int Primitive::underlyingParamCount() const
{
    return underlyingParamCounts[ int(_underlying) ];
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

void Primitive::clone(ObjectRef clonedObject) const
{
    using namespace std;

    PrimitiveRef clonedPrimitive = dynamic_pointer_cast<Primitive>(clonedObject);

    if (clonedPrimitive)
    {
        Identifiable::clone(clonedPrimitive);
        clonedPrimitive->setUnderlying(underlying());
    }
    else
    {
        throw runtime_error("clone() failed: expected Primitive - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
