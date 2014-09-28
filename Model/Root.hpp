#pragma once

#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Root : public DomainObject
{
public:
    static constexpr auto TYPE_NAME = "Root";
    virtual std::string objectTypeName() override { return TYPE_NAME; }

public:
    Root();
    virtual ~Root();

	NamespaceRef getNamespace();
	void setNamespace(NamespaceRef ns);

private:
	NamespaceRef _namespace;
};

typedef std::shared_ptr<Root> RootRef;

} } } // namespace: Everbase::InterfaceCompiler::Model
