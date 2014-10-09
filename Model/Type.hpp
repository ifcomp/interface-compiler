#pragma once

#include "Model/TypeBase.hpp"
#include "Model/Element.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Type : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Type();
    virtual ~Type();

    /**
     * @brief Clone Type object
     * @note This method clones Type object to UnresolvedType object!
     * @return UnresolvedType object
     */
    virtual ObjectRef clone() const override;

public:
    const ElementRef &primary() const;
    void setPrimary(const ElementRef &primary);

    void addParam(const ElementRef &param);
    std::vector<ElementRef> params() const;

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
    std::string getQualifiedName(const ElementRef &element) const;

private:
    ElementRef _primary;
    std::vector<ElementRef> _params;
};

typedef std::shared_ptr<Type> TypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
