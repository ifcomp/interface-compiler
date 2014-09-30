#pragma once

#include "MultiStream.hpp"
#include "CompilerSetup.hpp"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Everbase { namespace InterfaceCompiler {

class Compiler
{
public:
	void execute ( const CompilerSetup& setup, MultiStream<std::istream>& input, MultiStream<std::ostream>& output );
	void execute ( const CompilerSetup& setup, boost::filesystem::path input, boost::filesystem::path output );
};

} } // namespace: Everbase::InterfaceCompiler
