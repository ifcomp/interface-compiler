#include "Components/StandardParser.hpp"
#include "Components/Cpp/CppHeadersFormatter.hpp"
#include "Components/JavaScript/JSHeaderFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;
    using namespace std;

	ConfigProvider cprov;
	Components::StandardParser parser;

    if (true)
    {
        try {
            cout << "*** parsing file ***" << endl;

            std::ifstream file("../yaml/ecs.yaml");
            file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Model::RootRef root = parser.execute(cprov, file);

            try
            {
                std::ifstream configFile("../yaml/cpp.yaml");
                configFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

                std::ofstream outputFile("test.cpp", std::ios_base::trunc);
                outputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

                Components::CppHeadersFormatter format(configFile);
                format.execute(cprov, root, outputFile);
            }
            catch (const ios_base::failure &e)
            {
                cout << "error opening config file (" << e.what() << ")" << endl;
            }

            try
            {
                std::ifstream configFile("../yaml/js.yaml");
                configFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

                std::ofstream outputFile("test.js", std::ios_base::trunc);
                outputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

                Components::JSHeaderFormatter format(configFile);
                format.execute(cprov, root, outputFile);
            }
            catch (const ios_base::failure &e)
            {
                cout << "error opening config file (" << e.what() << ")" << endl;
            }
        }
        catch (const ios_base::failure &e)
        {
            cout << "[ERROR] could not read from file (" << e.what() << ")" << endl;
        }
        catch (const std::runtime_error &e)
        {
            cout << "[ERROR] " << e.what() << "please check your yaml file" << endl;
            return 1;
        }
    }
    else
    {
        std::cout << "need an objects.yaml and a config.yaml file" << endl;
        return 1;
    }

	return 0;
}
