#pragma once

#include "Model/Root.hpp"
#include "ConfigProvider.hpp"

#include <memory>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

class Parser
{
public:
	virtual Model::RootRef execute ( const ConfigProvider& cprov, std::istream& input ) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
