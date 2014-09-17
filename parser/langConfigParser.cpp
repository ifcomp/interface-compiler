#include "parser/langConfigParser.hpp"

using namespace std;
using namespace Api::Parser;


const char* LangConfigParser::KEY_TYPEMAP                  = "typemap";
const char* LangConfigParser::KEY_TYPE_PRIMITIVES          = "primitives";
const char* LangConfigParser::KEY_TYPE_CONTAINERS          = "containers";

const char* LangConfigParser::KEY_STYLE                    = "style";
const char* LangConfigParser::KEY_STYLE_CONTEXT            = "context";
const char* LangConfigParser::KEY_STYLE_NAMESTYLE          = "name-style";
const char* LangConfigParser::KEY_STYLE_NAMEDELIMITER      = "name-delimiter";
const char* LangConfigParser::KEY_STYLE_NAMEUSESHORT       = "name-use-short";

const char* LangConfigParser::styleContextKeys[] =
{
    "class", "constant", "container", "doc", "enum", "event", "namespace",
    "operation", "parameter", "primitive", "struct", "type", "value", NULL
};


LangConfigParser::LangConfigParser(std::string configFilename)
{
    mRootNode = loadFile(configFilename);
}


void LangConfigParser::parseTypeMap()
{
    if (checkNode(mRootNode, KEY_TYPEMAP, YAML::NodeType::Map, true))
    {
        if (checkNode(mRootNode[KEY_TYPEMAP], KEY_TYPE_PRIMITIVES, YAML::NodeType::Sequence, true))
        {
            for (auto primitiveNode : mRootNode[KEY_TYPEMAP][KEY_TYPE_PRIMITIVES])
            {
                if (primitiveNode.Type() == YAML::NodeType::Map)
                {
                    auto nodeIter = primitiveNode.begin();
                    mPrimitiveMap[ nodeIter->first.Scalar() ] = nodeIter->second.Scalar();
                }
                else
                {
                    throw runtime_error("bad primitive mapping\n");
                }
            }
        }
        if (checkNode(mRootNode[KEY_TYPEMAP], KEY_TYPE_CONTAINERS, YAML::NodeType::Sequence, true))
        {
            for (auto containerNode : mRootNode[KEY_TYPEMAP][KEY_TYPE_CONTAINERS])
            {
                if (containerNode.Type() == YAML::NodeType::Map)
                {
                    auto nodeIter = containerNode.begin();
                    mContainerMap[ nodeIter->first.Scalar() ] = nodeIter->second.Scalar();
                }
                else
                {
                    throw runtime_error("bad container mapping\n");
                }
            }
        }
    }
    cout << mPrimitiveMap.size() << " primitives, " << mContainerMap.size() << " containers" << endl;
}


std::string LangConfigParser::primitiveToLang(Api::Model::PrimitivePtr primitive)
{

}
