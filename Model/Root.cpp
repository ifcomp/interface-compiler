#include "Model/Root.hpp"
#include "Components/StandardParser.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Root::TYPE_NAME = "Root";

Root::Root()
{
}

Root::~Root()
{
}

ObjectRef Root::clone() const
{
    using namespace Everbase::InterfaceCompiler::Components;
    RootRef newRoot = std::make_shared<Root>();
    clone(newRoot);
    StandardParser::resolve(newRoot);
    return newRoot;
}

const NamespaceRef &Root::getNamespace() const
{
	return _namespace;
}

void Root::setNamespace(const NamespaceRef &ns)
{
    ns->setParent(shared_from_this());
    _namespace = ns;
}

void Root::clone(const ObjectRef &clonedObject) const
{
    using namespace std;

    RootRef clonedRoot = dynamic_pointer_cast<Root>(clonedObject);

    if (clonedRoot)
    {
        if (getNamespace())
        {
            clonedRoot->setNamespace(dynamic_pointer_cast<Namespace>(getNamespace()->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Root - got " + clonedObject->typeName());
    }
}

} } } // namespace: Everbase::InterfaceCompiler::Model
