#pragma once

#include "Config.hpp"
#include <string>

namespace Everbase { namespace InterfaceCompiler {

class ConfigProvider
{
public:
	Config load(std::string name) const;
};

} } // namespace: Everbase::InterfaceCompiler
