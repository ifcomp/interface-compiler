#include "Components/YamlReader.hpp"

using namespace std;


namespace Everbase { namespace InterfaceCompiler { namespace Components {

YamlReader::YamlReader()
{
}

YAML::Node YamlReader::loadFile(std::istream& stream)
{
    try {
        return YAML::Load(stream);
    }
    catch (const YAML::Exception &e)
    {
        throw runtime_error(std::string("Something's wrong with your YAML input file: ") + e.what());
    }
}


bool YamlReader::checkNode(const YAML::Node &node, const char *key, YAML::NodeType::value expectedType, bool mandatory)
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

} } } // namespace: Everbase::InterfaceCompiler::Components
