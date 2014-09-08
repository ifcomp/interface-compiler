#include "parser.hpp"

using namespace std;
using namespace Api;


Parser::Parser(Model::NamespacePtr rootNamespace) :
    mRootNamespace(rootNamespace)
{
}


bool Api::Parser::parseFile(std::string filename)
{
    mYamlConfig = YAML::LoadFile(filename);

    parseNode(mYamlConfig);
    return true;
}


void Api::Parser::setRootNamespace(Api::Model::NamespacePtr rootNamespace)
{
    mRootNamespace = rootNamespace;
}


void Api::Parser::parseNode(const YAML::Node &node)
{
    static int counter = 0;

    ++counter;
    std::string padding(counter * 2, '.');

    if (node.IsMap())
    {
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
        {
            const YAML::Node &key = it->first;
            const YAML::Node &value = it->second;

            if (key.Type() == YAML::NodeType::Scalar)
            {
                cout << padding << key.Scalar() << ": ";
                if (value.IsScalar())
                {
                    cout << value.Scalar();
                }
                else
                {
                    cout << " <value type = " << value.Type() << ">";
                }
                cout << endl;
            }
            else
            {
                cout << padding << "*** KEY TYPE: " << key.Type() << endl;
            }

            if (value.IsMap() || value.IsSequence())
            {
                parseNode(value);
            }
        }
    }
    else if (node.IsSequence())
    {
        for (int n = 0; n < node.size(); ++n)
        {
//            cout << padding << "parsing sequence node " << n << endl;
            cout << endl;
            parseNode(node[n]);
        }
    }
    else
    {
        cout << padding << "strange node " << node.Scalar() << endl;
    }

    --counter;
}

void Parser::parseNamespace(YAML::Node &node)
{

}
