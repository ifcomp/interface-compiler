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

class Object
{
public:
    Object();

    /**
     * @brief Get pointer to parent object.
     * @return Pointer to parent object
     */
    ObjectRef parent();

    /**
     * @brief Set pointer to parent object.
     * @param parent Pointer to parent object
     */
    void setParent(ObjectRef parent);

    /**
     * @brief Get Class identifier
     * @return Indentifier string
     */
    virtual std::string typeName() = 0;

private:
    ObjectRef _parent;
};

} } } // namespace Everbase::InterfaceCompiler::Model
