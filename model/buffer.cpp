#include "buffer.hpp"

using namespace Api::Model;

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

Buffer::BufferType Buffer::type()
{
	return mType;
}

void Buffer::setType(BufferType type)
{
	mType = type;
}
