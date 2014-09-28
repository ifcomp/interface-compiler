#pragma once

#include "Model/Root.hpp"
#include "ConfigProvider.hpp"

#include <memory>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

class Parser
{
public:
	std::shared_ptr<Model::Root> execute ( const ConfigProvider& cprov, std::istream& input ) const;
};

} } // namespace: Everbase::InterfaceCompiler
