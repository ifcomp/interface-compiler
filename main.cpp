#include "Components/TestFormatter.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;

	Components::TestFormatter t;
	t.execute(ConfigProvider(), Model::RootRef(), std::cout);

	return 0;
}
