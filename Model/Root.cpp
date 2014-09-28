#include "Model/Root.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Root::Root()
{
}

Root::~Root()
{
}

NamespaceRef Root::getNamespace()
{
	return _namespace;
}

void Root::setNamespace(NamespaceRef ns)
{
	_namespace = ns;
}

} } } // namespace: Everbase::InterfaceCompiler::Model
