#include "Components/StandardParser.hpp"
#include "Components/TestFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;

	if(argc < 2)
		throw std::runtime_error("argument missing");

	ConfigProvider cprov;
	Components::StandardParser parser;
	Components::TestFormatter format;

    std::ifstream file(argv[1]);
	file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	Model::RootRef root = parser.execute(cprov, file);
	format.execute(cprov, root, std::cout);

	return 0;
}
