#include "model/namespace.hpp"
#include "parser.hpp"

#include <iostream>
//#include <yaml-cpp/yaml.h>

using namespace std;
using namespace Api;

//void parseNode(const YAML::Node &node)
//{
//    static int counter = 0;

//    ++counter;
//    std::string padding(counter * 2, '.');

//    if (node.IsMap())
//    {
//        cout << padding << "node is a map" << endl;
//        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
//        {
//            const YAML::Node &key = it->first;
//            const YAML::Node &value = it->second;

//            if (key.Type() == YAML::NodeType::Scalar)
//            {
//                cout << padding << "key = '" << key.Scalar() << "'";
//                if (value.IsScalar())
//                {
//                    cout << " value = " << value.Scalar();
//                }
//                else
//                {
//                    cout << " value type = " << value.Type();
//                }
//                cout << endl;
//            }

//            if (value.IsMap() || value.IsSequence())
//            {
//                parseNode(value);
//            }
//        }
//    }
//    else if (node.IsSequence())
//    {
//        cout << padding << "node is a sequence" << endl;
////        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
////        {
////            cout << "parsing sequence node" << endl;
////            parseNode(it->second);
////        }
//        for (int n = 0; n < node.size(); ++n)
//        {
//            cout << padding << "parsing sequence node " << n << endl;
//            parseNode(node[n]);
//        }
//    }
//    else
//    {
//        cout << padding << "strange node " << node.Type() << endl;
//    }

//    --counter;
//}

int main()
{
//    try {
//        YAML::Node config = YAML::LoadFile("../api-generator/yaml/test.yaml");

//        parseNode(config);
//        std::fstream out("output.yaml", std::ios_base::out);
//        YAML::Emitter em(out);
//        em << config;




//        for (std::size_t n = 0; n < config.size(); ++n)
//        {
//                cout << "node " << n << endl;

//           if (config[n].IsMap())
//            {
//                for (auto it = config[n].begin(); it != config[n].end(); ++it)
//                {
//                    YAML::Node key = it->first;
//                    YAML::Node value = it->second;
//                    if (key.Type() == YAML::NodeType::Scalar)
//                    {
//                        // This should be true; do something here with the scalar key.
//                        cout << "key = " << key.Scalar() << " value type = " << value.Type() << endl;
//                    }
//                    if (value.Type() == YAML::NodeType::Map)
//                    {
//                        // This should be true; do something here with the map.
//                        cout << "***map of " << key.Scalar() << endl;
//                    }
//                }
//            }
//            else
//            {
//                cout << "no map" << endl;
//            }
//        }

//        for (std::size_t n = 0; n < config.size(); ++n)
//        {
//            if (config[n])
//            {
//                cout << config[n].Scalar() << config[n].Type() << endl;
//            }
//            else
//            {
//                cout << "undefined node" << endl;
//            }
//        }

//    }
//    catch (YAML::Exception e)
//    {
//        cout << "OOPS: " << e.what() << endl;
//    }

    Model::NamespacePtr rootNamespace(new Model::Namespace);
    rootNamespace->setLongName("::");
    Parser parser(rootNamespace);
    parser.parseFile("../api-generator/yaml/test.yaml");

    cout << "done" << endl;
    return 0;
}

