#include "model/documentation.hpp"

using namespace Api::Model;

Documentation::Documentation()
{
}

Documentation::~Documentation()
{
}

std::string Documentation::brief()
{
	return mBrief;
}

void Documentation::setBrief(std::string brief)
{
	mBrief = brief;
}

std::string Documentation::more()
{
	return mMore;
}

void Documentation::setMore(std::string more)
{
    mMore = more;
}

const std::vector<std::list<std::string> > &Documentation::docEntries()
{

}

void Documentation::addDocEntry(std::string doxygenKey, std::list<std::string> elements)
{

}

void Documentation::addDocEntry(std::string doxygenKey, std::string paramName, std::string description)
{

}
