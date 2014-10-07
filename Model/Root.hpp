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
    virtual ObjectRef clone() const override;

public:
    NamespaceRef getNamespace() const;
	void setNamespace(NamespaceRef ns);

protected:
    void clone(ObjectRef clonedObject) const override;

private:
	NamespaceRef _namespace;
};

typedef std::shared_ptr<Root> RootRef;

} } } // namespace: Everbase::InterfaceCompiler::Model
