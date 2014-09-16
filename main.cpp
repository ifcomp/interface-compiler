#include "model/namespace.hpp"
#include "parser.hpp"
#include "generator/cppGenerator.hpp"

using namespace std;
using namespace Api;

int main(int argc, char **argv)
{
    Model::NamespacePtr rootNamespace(new Model::Namespace("::"));
    Parser parser(rootNamespace);

    if (argc >= 2)
    {
        for (int n = 1; n < argc; ++n)
        {
            try {
                cout << "*** parsing file " << argv[n] << " ***" << endl;

                parser.parseFile(argv[n]);
            }
            catch (const YAML::Exception &e)
            {
                cout << "Something's wrong with your YAML input file: " << e.what() << endl;
                return 1;
            }
            catch (const runtime_error &e)
            {
                cout << "[PARSE ERROR] " << e.what() << "please check your yaml file " << argv[n] << endl;
                return 1;
            }
        }

        parser.resolveTypesInNamespace(rootNamespace);
        parser.listKnownTypes();

        Gen::CppGenerator generator(rootNamespace, "");

        cout << "done" << endl;
        return 0;
    }
    else
    {
        cout << "gimme some YAML to chew on!" << endl;
        return 1;
    }

//    parser.parseFile("../api-generator/yaml/test.yaml");

}

