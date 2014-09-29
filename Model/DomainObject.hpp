#pragma once

#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

/**
 * @brief Base class for identifying and linking domain objects.
 * @autor Gunther Lemm <lemm@silpion.de>
 */
class DomainObject;

typedef std::shared_ptr<DomainObject> DomainObjectRef;

class DomainObject
{
public:
    DomainObject(DomainObjectRef parentObject = nullptr);

    /**
     * @brief Get pointer to parent object.
     * @return Pointer to parent object
     */
    DomainObjectRef parentObject();

    /**
     * @brief Set pointer to parent object.
     * @param parentObject Pointer to parent object
     */
    void setParentObject(DomainObjectRef parentObject);

    /**
     * @brief Get Class identifier
     * @return Indentifier string
     */
    virtual std::string objectTypeName() = 0;

private:
    DomainObjectRef _parentObject;
};

} } } // namespace Everbase::InterfaceCompiler::Model
