#pragma once

#include "Model/Object.hpp"
#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Root : public Object
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Root();
    virtual ~Root();

public:
	NamespaceRef getNamespace();
	void setNamespace(NamespaceRef ns);

private:
	NamespaceRef _namespace;
};

typedef std::shared_ptr<Root> RootRef;

} } } // namespace: Everbase::InterfaceCompiler::Model
