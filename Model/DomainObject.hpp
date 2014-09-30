#pragma once

#include <memory>
#include <vector>
#include <map>

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

protected:

    /**
     * @brief Return map as vector
     */
    template <typename T> std::vector<T> mapToVector(const std::map<std::string, T> &map)
    {
        std::vector<T> result;

        for (auto value : map)
        {
            result.push_back(value.second);
        }

        return result;
    }


private:
    DomainObjectRef _parentObject;
};

} } } // namespace Everbase::InterfaceCompiler::Model
