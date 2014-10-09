#pragma once

#include <memory>
#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

/**
 * @brief Base class for identifying and linking domain objects.
 * @autor Gunther Lemm <lemm@silpion.de>
 */
class Object;

typedef std::shared_ptr<Object> ObjectRef;

class Object : public std::enable_shared_from_this<Object>
{
public:
    Object();

    /**
     * @brief Get pointer to parent object.
     * @return Pointer to parent object
     */
    const ObjectRef &parent() const;

    /**
     * @brief Set pointer to parent object.
     * @param parent Pointer to parent object
     */
    void setParent(const ObjectRef &parent);

    /**
     * @brief Get Class identifier
     * @return Indentifier string
     */
    virtual std::string typeName() const = 0;

    /**
     * @brief Clone current object.
     * @return Pointer to new instance of object's class.
     */
    virtual ObjectRef clone() const = 0;

protected:

    /**
     * @brief Worker method that actually clones current instance.
     * @param clonedObject Empty object where current instance is copied.
     */
    virtual void clone(const ObjectRef &clonedObject) const {}

private:
    ObjectRef _parent;
};

} } } // namespace Everbase::InterfaceCompiler::Model
