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
    return mLongName;
}

void Identifiable::setLongName(std::string longName)
{
    mLongName = longName;
}

std::string Identifiable::shortName()
{
    return mShortName;
}

void Identifiable::setShortName(std::string shortName)
{
    mShortName = shortName;
}

DocumentationRef Identifiable::doc()
{
	return mDoc;
}

void Identifiable::setDoc(DocumentationRef doc)
{
	mDoc = doc;
}

} } } // namespace Everbase::InterfaceCompiler::Model
