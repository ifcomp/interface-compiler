#include "model/namespace.hpp"
#include "parser.hpp"

#include <iostream>
//#include <yaml-cpp/yaml.h>

using namespace std;
using namespace Api;

int main()
{
    Model::NamespacePtr rootNamespace(new Model::Namespace);
    rootNamespace->setLongName("::");
    Parser parser(rootNamespace);
    parser.parseFile("../api-generator/yaml/test.yaml");

    cout << "done" << endl;
    return 0;
}

