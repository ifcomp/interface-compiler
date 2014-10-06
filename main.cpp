#include "Components/StandardParser.hpp"
#include "Components/Cpp/FwdDeclFormatter.hpp"
#include "Components/Cpp/HeaderFormatter.hpp"
#include "Components/JavaScript/HeaderFormatter.hpp"
#include "Components/JavaScript/WebClientFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;
    using namespace std;

    try
    {
        std::ifstream input(argc > 1 ? argv[1] : "../yaml/everbase.yaml");
        input.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

        Components::StandardParser parser;
        Model::RootRef root = parser.execute(input);

        try
        {
            std::ofstream output("everbase-fwddecl.cpp", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Components::Cpp::FwdDeclFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

        try
        {
            std::ofstream output("everbase-header.cpp", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Components::Cpp::HeaderFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

        try
        {
            std::ofstream output("everbase-header.js", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Components::JavaScript::HeaderFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

		try
        {
            std::ofstream output("everbase-web-client.js", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

			Components::JavaScript::WebClientFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }
    }
    catch (const ios_base::failure &e)
    {
        cout << "[ERROR] could not read from file (" << e.what() << ")" << endl;
    }
    catch (const std::runtime_error &e)
    {
        cout << "[ERROR] " << e.what() << " - please check your yaml file" << endl;
        return 1;
    }

	return 0;
}
