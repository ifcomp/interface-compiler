#include "Components/StandardParser.hpp"
#include "Components/TestFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;
    using namespace std;

	ConfigProvider cprov;
	Components::StandardParser parser;


    if (argc == 3)
    {
        try {
            cout << "*** parsing file " << argv[1] << " ***" << endl;

            std::ifstream file(argv[1]);
            file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Model::RootRef root = parser.execute(cprov, file);

            try {
                std::ifstream configFile(argv[2]);
                configFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

                Components::TestFormatter format(configFile);
                format.execute(cprov, root, std::cout);
            }
            catch (const ios_base::failure &e)
            {
                cout << "error opening config file " << argv[2] << " (" << e.what() << ")" << endl;
            }
        }
        catch (const ios_base::failure &e)
        {
            cout << "could not read from file " << argv[1] << " (" << e.what() << ")" << endl;
        }
        catch (const std::runtime_error &e)
        {
            cout << "[PARSE ERROR] " << e.what() << "please check your yaml file " << argv[1] << endl;
            return 1;
        }
    }
    else
    {
        std::cout << "need an objects.yalm and a config.yaml file" << endl;
        return 1;
    }

	return 0;
}
