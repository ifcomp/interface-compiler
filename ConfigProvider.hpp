#pragma once

#include "Config.hpp"
#include <string>

namespace Everbase { namespace InterfaceCompiler {

class ConfigProvider
{
public:
	/*virtual*/ Config load(std::string name) const /*= 0*/;
};

} } // namespace: Everbase::InterfaceCompiler
