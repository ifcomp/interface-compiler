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

const char  LangConfigParser::TYPE_PLACEHOLDER             = '@';

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
    string typeName = primitive->typeName();

    if (mPrimitiveMap.find(typeName) == mPrimitiveMap.end())
    {
        throw runtime_error("primitive type " + typeName + " not declared in language config!\n");
    }
    return mPrimitiveMap[typeName];
}


string LangConfigParser::containerToLang(Api::Model::ContainerPtr container)
{
    string typeName = container->typeName();

    if (mContainerMap.find(typeName) == mContainerMap.end())
    {
        throw runtime_error("container type " + typeName + " not declared in language config!\n");
    }
    return mContainerMap[typeName];
}


string LangConfigParser::containerTypeToLang(Model::TypePtr type)
{
    if (Model::ResolvedTypePtr resolvedType = dynamic_pointer_cast<Model::ResolvedType>(type))
    {
        string output;

        if (Model::ContainerPtr container = dynamic_pointer_cast<Model::Container>(resolvedType->primary()))
        {
            output = containerToLang(container);

            vector<string> params;

            // resolve subtype strings
            for (Model::NamespaceMemberPtr param : resolvedType->params())
            {
                if (Model::PrimitivePtr primitive = dynamic_pointer_cast<Model::Primitive>(param))
                {
                    params.push_back(primitiveToLang(primitive));
                }
                else
                {
                    ///< @todo: how do we know if we need to add a namespace?
                    params.push_back(param->longName());
                }
            }

            // replace subtype placeholders
            size_t pos = 0;
            int paramCount = 0;

            while ((pos = output.find(TYPE_PLACEHOLDER, pos)) != string::npos)
            {
                output.replace(pos, 1, params.at(paramCount));
                ++paramCount;
            }

            if (paramCount != params.size())
            {
                throw runtime_error("LangConfigParser::containerTypeToLang(): subtype count in " +
                                    resolvedType->primary()->longName() +
                                    " definition does not match language-specific config!\n");
            }
            return output;
        }
        throw runtime_error("LangConfigParser::containerTypeToLang(): primary is not a container!\n");
    }
    throw runtime_error("LangConfigParser::containerTypeToLang(): bad type!\n");
}
