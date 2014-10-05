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
    ObjectRef parentObject();

    /**
     * @brief Set pointer to parent object.
     * @param parentObject Pointer to parent object
     */
    void setParentObject(ObjectRef parentObject);

    /**
     * @brief Get Class identifier
     * @return Indentifier string
     */
    virtual std::string objectTypeName() = 0;

private:
    ObjectRef _parentObject;
};

} } } // namespace Everbase::InterfaceCompiler::Model
