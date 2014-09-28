#include "Model/Documentation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

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
	return mDocEntries;
}

void Documentation::addDocEntry(std::string doxygenKey, std::list<std::string> elements)
{
	elements.insert(elements.begin(), doxygenKey);
	mDocEntries.push_back(elements);
}

void Documentation::addDocEntry(std::string doxygenKey, std::string paramName, std::string description)
{
	mDocEntries.push_back(std::list<std::string>{ doxygenKey, paramName, description });
}

} } } // namespace Everbase::InterfaceCompiler::Model
