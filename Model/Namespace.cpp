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

std::vector<ElementRef> Namespace::elements()
{
    return _elements;
}

void Namespace::addElement(ElementRef element)
{
	_elements.push_back(element);
}

} } } // namespace Everbase::InterfaceCompiler::Model
