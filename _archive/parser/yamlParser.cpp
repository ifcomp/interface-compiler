#include "parser/yamlParser.hpp"

using namespace std;
using namespace Api::Parser;

YamlParser::YamlParser()
{
}


YAML::Node YamlParser::loadFile(string filename)
{
    try {
        return YAML::LoadFile(filename);
    }
    catch (const YAML::Exception &e)
    {
        throw runtime_error("Something's wrong with your YAML input file: " + filename + " : " + e.what());
    }
}


bool YamlParser::checkNode(const YAML::Node &node, const char *key, YAML::NodeType::value expectedType, bool mandatory)
{
    if (node[key].Type() == expectedType)
    {
        return true;
    }

    if (!node[key].IsDefined())
    {
        if (mandatory)
        {
            throw runtime_error("definition of key '" + string(key) + "' is missing!\n");
        }
    }
    else
    {
        throw runtime_error("bad definition of key '" + string(key) + "'\n");
    }

    return false;
}
