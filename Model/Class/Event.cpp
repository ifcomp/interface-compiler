#include "Model/Class/Event.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::Event::TYPE_NAME = "Event";

Class::Event::Event()
{
}

Class::Event::~Event()
{
}

void Class::Event::addValue(ParameterRef value)
{
    _values.push_back(value);
}

std::vector<ParameterRef> Class::Event::values()
{
    return _values;
}

void Class::Event::setTypeId(boost::uuids::uuid typeId)
{
	_typeId = typeId;
}

boost::uuids::uuid Class::Event::typeId()
{
	return _typeId;
}

} } } // namespace Everbase::InterfaceCompiler::Model
