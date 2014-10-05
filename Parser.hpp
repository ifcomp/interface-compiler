#pragma once

#include "Model/Root.hpp"

#include <memory>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

class Parser
{
public:
	virtual Model::RootRef execute ( std::istream& input ) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
