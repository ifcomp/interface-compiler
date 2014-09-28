#pragma once

#include "Model/Root.hpp"
#include "ConfigProvider.hpp"

#include <string>
#include <map>
#include <memory>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

class Transformer
{
public:
	virtual std::map<std::string, Model::RootRef> execute ( const ConfigProvider& cprov, Model::RootRef input ) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
