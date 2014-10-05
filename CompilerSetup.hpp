#pragma once

#include "Parser.hpp"
#include "Transformer.hpp"
#include "Formatter.hpp"

#include <list>
#include <memory>

namespace Everbase { namespace InterfaceCompiler {

struct CompilerSetup
{
	std::shared_ptr < Parser > parser;
	std::list < std::shared_ptr < Transformer > > transformer;
	std::list < std::shared_ptr < Formatter > > formatter;
};

} } // namespace: Everbase::InterfaceCompiler
