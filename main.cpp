#include "Components/StandardParser.hpp"

#include "Components/Cpp/HeaderFormatter.hpp"
#include "Components/Cpp/LibraryHeaderFormatter.hpp"
#include "Components/Cpp/LibraryFormatter.hpp"
#include "Components/Cpp/JsonEncodingFormatter.hpp"

#include "Components/JavaScript/HeaderFormatter.hpp"
#include "Components/JavaScript/WebClientFormatter.hpp"

#include <iostream>
#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;
    using namespace std;

    // Check parameter count
    if( argc < 4 )
    {
        cerr << "Usage: " << argv[0] << " <input> <formatter 1> <output 1> ..." << endl;
        cerr << "Formatter: C++-Header" << endl;
        cerr << "           C++-LibraryHeader" << endl;
        cerr << "           C++-Library" << endl;
        cerr << "           C++-JsonEncoding" << endl;
        cerr << "           C++-WebService" << endl;
        cerr << "           C++-Kernel" << endl;
        cerr << "           Js-Header" << endl;
        cerr << "           Js-WebClient" << endl;
        return 1;
    }

    // Get parameter
    std::string inputPath = argv[1];

    std::vector<std::pair<std::string, std::string>> formats;

    for( int i = 2; (i + 1) < argc; i += 2 )
    {
        formats.push_back( std::pair<std::string, std::string> { argv[i], argv[i+1] } );
    }

    // Open file streams
    std::map<std::string, std::unique_ptr<std::ofstream>> outstreams;

    try
    {
        for( auto i : formats )
        {
            if( outstreams.find(i.second) != outstreams.end() )
                { continue; }

            outstreams.emplace( i.second, std::unique_ptr<std::ofstream>(new std::ofstream(i.second, ios_base::trunc)) );
            outstreams[i.second]->exceptions( ifstream::failbit | ifstream::badbit );
        }
    }
    catch (const exception& e)
    {
        cerr << "[ERROR 1] " << e.what() << endl;
        return 1;
    }

    // Parse sources

    Model::RootRef root;

    try
    {
        ifstream input(inputPath.c_str());
        input.exceptions ( ifstream::failbit | ifstream::badbit );

        Components::StandardParser parser;
        root = parser.execute(input);
    }
    catch (const exception& e)
    {
        cerr << "[ERROR 2] " << e.what() << endl;
        return 1;
    }

    // Format

    try
    {
        for( auto format : formats )
        {
            ofstream& output = *(outstreams[format.second]);

            if( format.first == "C++-Header" )
            {
                Components::Cpp::HeaderFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-LibraryHeader" )
            {
                Components::Cpp::LibraryHeaderFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-Library" )
            {
                Components::Cpp::LibraryFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-JsonEncoding" )
            {
                Components::Cpp::JsonEncodingFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-WebService" )
            {
                throw std::runtime_error("not implemented");
            }
            else
            if( format.first == "C++-Kernel" )
            {
                throw std::runtime_error("not implemented");
            }
            else
            if( format.first == "Js-Header" )
            {
                Components::JavaScript::HeaderFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Js-WebClient" )
            {
                Components::JavaScript::WebClientFormatter format;
                format.execute(root, output);
            }
            else
                { throw std::runtime_error(std::string("invalid format: ") + format.first); }
        }
    }
    catch (const exception& e)
    {
        cerr << "[ERROR 3] " << e.what() << endl;
        return 1;
    }

	return 0;
}
