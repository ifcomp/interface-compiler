#pragma once

#include "Model/NamespaceMember.hpp"

#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Primitive : public NamespaceMember
{
public:

    static const char* TYPE_NAME;

    enum class Underlying
    {
        UNDEFINED = -1,
        BYTE,
        UINT16,
        UINT32,
        UINT64,
        BOOLEAN,
        TIMESTAMP,
        STRING,
        UUID,
        BUFFER,
        CONST_BUFFER,
        VECTOR,
        LIST,
        SET,
        _COUNT_       ///< Number of enum entries. THIS MUST BE THE LAST ENTRY!
    };

    /**
     * @brief Mapping of underlying name strings to Underlying enum
     */
    static const char *underlyingNames[int(Underlying::_COUNT_)];

private:
    Underlying _underlying;

public:
    Primitive();
    virtual ~Primitive();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    /**
     * @brief Get Underlying
     * @return Current Underlying
     */
    Underlying underlying();

    /**
     * @brief Set underlying by Underlying.
     * @see Underlying
     * @param underlying Underlying to set
     */
    void setUnderlying(Underlying underlying);

    /**
     * @brief Get underlying name
     * @return Type name
     */
    std::string underlyingName();

    /**
     * @brief Set underlying by underlying name.
     * @see underlyingNames
     * @param underlyingName Type name as listed in underlyingNames
     * @throw std::runtime_error if name was not found
     */
    void setUnderlying(std::string underlyingName);

    /**
     * @brief Decode underlying name to Underlying
     * @param underlyingName Type name as listed in underlyingNames
     * @return Returns Underlying (Underlying::UNDEFINED if name was not found)
     */
    static Underlying decodeUnderlyingName(std::string underlyingName);

    /**
     * @brief Return human-readable list of known underlyings.
     * @return String containing a list of underlying names, each one in a row.
     */
    static std::string listSupportedUnderlying();
};

typedef std::shared_ptr<Primitive> PrimitiveRef;

} } } // namespace Everbase::InterfaceCompiler::Model
