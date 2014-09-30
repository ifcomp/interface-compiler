#pragma once

#include "MultiStream.hpp"
#include "CompilerSetup.hpp"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Everbase { namespace InterfaceCompiler {

template<typename StreamT>
class MultiStream
{
public:
	virtual StreamT& stream(std::string name) = 0;
};

} } // namespace: Everbase::InterfaceCompiler
