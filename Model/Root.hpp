#pragma once

#include "Model/Object.hpp"
#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Root : public Object
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Root();
    virtual ~Root();

    /**
     * @brief Clone object tree
     *
     * This method clones the complete object tree. In the first step, all
     * types are copied as unresoved types. In the second step, types are
     * resolved to objects contained in the tree.
     *
     * @return Pointer to copy of object tree
     */
    virtual ObjectRef clone() const override;

public:
    const NamespaceRef &getNamespace() const;
    void setNamespace(const NamespaceRef &ns);

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
	NamespaceRef _namespace;
};

typedef std::shared_ptr<Root> RootRef;

} } } // namespace: Everbase::InterfaceCompiler::Model
