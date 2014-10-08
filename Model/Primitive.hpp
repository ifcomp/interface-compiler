#pragma once

#include "Model/Element.hpp"

#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Primitive : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
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
//        BUFFER,
//        CONST_BUFFER,
        VECTOR,
        LIST,
        SET,
        MAP,
        _COUNT_       ///< Number of enum entries. THIS MUST BE THE LAST ENTRY!
    };

    /**
     * @brief Mapping of underlying name strings to Underlying enum
     */
    static const char *underlyingNames[int(Underlying::_COUNT_)];

    static const unsigned int underlyingParamCounts[int(Underlying::_COUNT_)];

public:
    Primitive();
    virtual ~Primitive();
    virtual ObjectRef clone() const override;

public:
    /**
     * @brief Get Underlying
     * @return Current Underlying
     */
    Underlying underlying() const;

    /**
     * @brief Set underlying by Underlying.
     * @see Underlying
     * @param underlying Underlying to set
     */
    void setUnderlying(Underlying underlying);

    /**
     * @brief Get underlying name
     * @return std::string
     */
    std::string underlyingName() const;

    /**
     * @brief Get underlying parameter count
     * @return unsigned int
     */
    unsigned int underlyingParamCount() const;

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

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    Underlying _underlying;
};

typedef std::shared_ptr<Primitive> PrimitiveRef;

} } } // namespace Everbase::InterfaceCompiler::Model
