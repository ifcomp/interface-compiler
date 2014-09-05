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
