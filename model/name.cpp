#include "model/name.hpp"

using namespace Api::Model;

Name::Name()
{
}

Name::~Name()
{
}

std::string Name::longName()
{
	return mLongName;
}

void Name::setLongName(std::string longName)
{
	mLongName = longName;
}

std::string Name::shortName()
{
	return mShortName;
}

void Name::setShortName(std::string shortName)
{
	mShortName = shortName;
}
