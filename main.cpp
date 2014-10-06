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

    /*
    // Read and validate input
    if( argc < 4 )
    {
        cerr << "Usage: " << argv[0] << " <input> <formatter 1> <output 1> ..." << endl;
        cerr << "Formatter: c++-header c++-kernel c++-library c++-webservice" << endl;
        cerr << "           js-header js-webclient" << endl;
        return -1;
    }

    std::string inputPath = argv[1];

    std::vector<std::pair<std::string, std::string>> formats;

    for( std::size_t i = 2; (i + 1) < argc; ++i )
    {
        formats.push_back( std::pair<std::string, std::string> { argv[i], argv[i+1] } ):
    }

    // Open file streams
    std::map<std::string, std::ostream> outstreams;

    try
    {

        for( auto i : formats )
        {
            if( outstreams.find(i.second) != outstreams.end() )
                { continue; }

            outstreams.emplace( i.second, i.second, ios_base::trunc );
            outstreams[i.second].exceptions ( ifstream::failbit | ifstream::badbit );
        }
    }

    // Parse 
    */

    try
    {
        ifstream input(argc > 1 ? argv[1] : "../yaml/everbase.yaml");
        input.exceptions ( ifstream::failbit | ifstream::badbit );

        Components::StandardParser parser;
        Model::RootRef root = parser.execute(input);

        try
        {
            ofstream output("everbase-fwddecl.cpp", ios_base::trunc);
            output.exceptions ( ifstream::failbit | ifstream::badbit );

            Components::Cpp::FwdDeclFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

        try
        {
            ofstream output("everbase-header.cpp", ios_base::trunc);
            output.exceptions ( ifstream::failbit | ifstream::badbit );

            Components::Cpp::HeaderFormatter format;
            format.execute(root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

        try
        {
            ofstream output("everbase-header.js", ios_base::trunc);
            output.exceptions ( ifstream::failbit | ifstream::badbit );

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
    catch (const runtime_error &e)
    {
        cout << "[ERROR] " << e.what() << " - please check your yaml file" << endl;
        return 1;
    }

	return 0;
}
