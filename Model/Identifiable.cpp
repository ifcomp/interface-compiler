#include "Model/Identifiable.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Identifiable::Identifiable(DomainObjectRef parentObject)
    : DomainObject(parentObject)
{
}

Identifiable::~Identifiable()
{
}

std::string Identifiable::longName()
{
    return _longName;
}

void Identifiable::setLongName(std::string longName)
{
    _longName = longName;
}

std::string Identifiable::shortName()
{
    return _shortName;
}

void Identifiable::setShortName(std::string shortName)
{
    _shortName = shortName;
}

DocumentationRef Identifiable::doc()
{
	return _doc;
}

void Identifiable::setDoc(DocumentationRef doc)
{
	_doc = doc;
}

} } } // namespace Everbase::InterfaceCompiler::Model
