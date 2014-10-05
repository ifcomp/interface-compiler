#include "Model/Type.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Type::TYPE_NAME = "Type";

Type::Type()
{
}

Type::~Type()
{
}

ElementRef Type::primary()
{
    return _primary;
}

void Type::setPrimary(ElementRef primary)
{
    _primary = primary;
}

void Type::addParam(ElementRef param)
{
    _params.push_back(param);
}

const std::vector<ElementRef>& Type::params()
{
    return _params;
}

} } } // namespace Everbase::InterfaceCompiler::Model
