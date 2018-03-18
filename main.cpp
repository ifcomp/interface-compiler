#include "Components/StandardParser.hpp"

#include "Components/Cpp/HeaderFormatter.hpp"
#include "Components/Cpp/LibraryHeaderFormatter.hpp"
#include "Components/Cpp/LibraryFormatter.hpp"
#include "Components/Cpp/RpcFormatter.hpp"
#include "Components/Cpp/RpcBinaryFormatter.hpp"
#include "Components/Cpp/RpcJsonFormatter.hpp"

#include "Components/JavaScript/HeaderFormatter.hpp"
#include "Components/JavaScript/WebClientFormatter.hpp"
#include "Components/JavaScript/JsonEncoding.hpp"

#include "Components/ObjectiveC/HeaderFormatter.hpp"
#include "Components/ObjectiveC/WrapperFormatter.hpp"

#include "Components/Ruby/NamespacesFormatter.hpp"
#include "Components/Ruby/EnumsFormatter.hpp"
#include "Components/Ruby/StructsFormatter.hpp"
#include "Components/Ruby/ClassesFormatter.hpp"
#include "Components/Ruby/EventsFormatter.hpp"
#include "Components/Ruby/OperationsFormatter.hpp"
#include "Components/Ruby/ConstantsFormatter.hpp"

#include "Components/TypeScript/HeaderFormatter.hpp"
#include "Components/TypeScript/WebClientFormatter.hpp"
#include "Components/TypeScript/JsonEncoding.hpp"

#include <iostream>
#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		printf("arg %d: %s\n", i, argv[i]);
	};


	using namespace Everbase::InterfaceCompiler;
    using namespace std;

    // Check parameter count
    if( argc < 4 )
    {
        cerr << "Usage: " << argv[0] << " <input> <formatter 1> <output 1> ..." << endl;
        cerr << "Formatter: C++-Header" << endl;
        cerr << "           C++-LibraryHeader" << endl;
        cerr << "           C++-Library" << endl;
        cerr << "           C++-Rpc" << endl;
        cerr << "           C++-RpcBinary" << endl;
        cerr << "           C++-RpcJson" << endl;
        cerr << "           C++-WebService" << endl;
        cerr << "           C++-Kernel" << endl;
        cerr << "           ObjC-Header" << endl;
        cerr << "           ObjC-Wrapper" << endl;
        cerr << "           Js-Header" << endl;
        cerr << "           Js-JsonEncoding" << endl;
        cerr << "           Js-WebClient" << endl;
        cerr << "           Ruby-Namespaces" << endl;
        cerr << "           Ruby-Enums" << endl;
        cerr << "           Ruby-Structs" << endl;
        cerr << "           Ruby-Classes" << endl;
        cerr << "           Ruby-Events" << endl;
        cerr << "           Ruby-Operations" << endl;
        cerr << "           Ruby-Constants" << endl;
        cerr << "           Ts-Header" << endl;
        cerr << "           Ts-JsonEncoding" << endl;
        cerr << "           Ts-WebClient" << endl;
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
    catch (const runtime_error& e)
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
    catch (const runtime_error& e)
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
            if( format.first == "C++-Rpc" )
            {
                Components::Cpp::RpcFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-RpcBinary" )
            {
                Components::Cpp::RpcBinaryFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "C++-RpcJson" )
            {
                Components::Cpp::RpcJsonFormatter format;
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
            if( format.first == "ObjC-Header" )
            {
                Components::ObjectiveC::HeaderFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "ObjC-Wrapper" )
            {
                Components::ObjectiveC::WrapperFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Js-Header" )
            {
                Components::JavaScript::HeaderFormatter format;
                format.execute(root, output);
            }
            else
            if (format.first == "Js-JsonEncoding")
            {
                Components::JavaScript::JsonEncoding format;
                format.execute(root, output);
            }
            else
            if( format.first == "Js-WebClient" )
            {
                Components::JavaScript::WebClientFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Namespaces" )
            {
                Components::Ruby::NamespacesFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Enums" )
            {
                Components::Ruby::EnumsFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Structs" )
            {
                Components::Ruby::StructsFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Classes" )
            {
                Components::Ruby::ClassesFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Events" )
            {
                Components::Ruby::EventsFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Operations" )
            {
                Components::Ruby::OperationsFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ruby-Constants" )
            {
                Components::Ruby::ConstantsFormatter format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ts-Header" )
            {
                Components::TypeScript::HeaderFormatter format;
                format.execute(root, output);
            }
            else
            if (format.first == "Ts-JsonEncoding")
            {
                Components::TypeScript::JsonEncoding format;
                format.execute(root, output);
            }
            else
            if( format.first == "Ts-WebClient" )
            {
                Components::TypeScript::WebClientFormatter format;
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
