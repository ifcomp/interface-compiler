#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "model/baseObject.hpp"

class Primitive : public BaseObject
{
public:
    enum PrimitiveType
    {
        BYTE,
        UINT32,
        UINT64,
        FLOAT,
        BOOLEAN,
        TIMESTAMP,
        STRING,
        UUID
    };

private:
    PrimitiveType mType;
    std::string mMapTo;

public:
    Primitive();

    PrimitiveType type();
    void setType(PrimitiveType type);

    std::string mapTo();
    void setMapTo(std::string mapTo);

};

#endif // PRIMITIVE_HPP
