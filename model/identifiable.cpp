#include "model/identifiable.hpp"

using namespace Api::Model;

Identifiable::Identifiable()
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

DocumentationPtr Identifiable::doc()
{
	return mDoc;
}

void Identifiable::setDoc(DocumentationPtr doc)
{
	mDoc = doc;
}
