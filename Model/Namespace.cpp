#include "Model/Namespace.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Namespace::TYPE_NAME = "Namespace";

Namespace::Namespace()
{
}

Namespace::Namespace(std::string longName)
{
    setLongName(longName);
}

Namespace::~Namespace()
{
}

ObjectRef Namespace::clone() const
{
    NamespaceRef newNamespace = std::make_shared<Namespace>();
    clone(newNamespace);
    return newNamespace;
}

std::vector<ElementRef> Namespace::elements() const
{
    return _elements;
}

void Namespace::clone(ObjectRef clonedObject) const
{
    using namespace std;

    NamespaceRef clonedNamespace = dynamic_pointer_cast<Namespace>(clonedObject);

    if (clonedNamespace)
    {
        Identifiable::clone(clonedNamespace);

        for (auto element : elements())
        {
            clonedNamespace->addElement(dynamic_pointer_cast<Element>(element->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Namespace - got " + clonedObject->typeName());
    }
}

void Namespace::addElement(ElementRef element)
{
	_elements.push_back(element);
}

} } } // namespace Everbase::InterfaceCompiler::Model
