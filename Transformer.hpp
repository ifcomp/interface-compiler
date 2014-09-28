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
	std::map<std::string, std::shared_ptr<Model::Root>> execute ( const ConfigProvider& cprov, std::shared_ptr<Model::Root> input ) const;
};

} } // namespace: Everbase::InterfaceCompiler
