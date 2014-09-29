#include "Components/StandardParser.hpp"
#include "Components/TestFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;

	ConfigProvider cprov;
	Components::StandardParser parser;
	Components::TestFormatter format;

    std::ifstream file("../api-generator/_archive/yaml/test.yaml");
	file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	Model::RootRef root = parser.execute(cprov, file);
	format.execute(cprov, root, std::cout);

	return 0;
}
