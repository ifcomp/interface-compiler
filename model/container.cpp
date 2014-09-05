#include "container.hpp"

using namespace Api::Model;

Container::Container()
{
}

Container::~Container()
{
}

Container::ContainerType Container::type()
{
	return mType;
}

void Container::setType(Container::ContainerType type)
{
	mType = type;
}
