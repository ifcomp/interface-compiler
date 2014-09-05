#include "model/identifiable.hpp"

using namespace Api::Model;

Identifiable::Identifiable()
{
}

Identifiable::~Identifiable()
{
}

Name Identifiable::name()
{
	return mName;
}

void Identifiable::setName(Name name)
{
	mName = name;
}

DocumentationPtr Identifiable::doc()
{
	return mDoc;
}

void Identifiable::setDoc(DocumentationPtr doc)
{
	mDoc = doc;
}
