#include "parser/langConfigParser.hpp"

#include "model/class.hpp"
#include "model/constant.hpp"
#include "model/container.hpp"
#include "model/documentation.hpp"
#include "model/enum.hpp"
#include "model/event.hpp"
#include "model/namespace.hpp"
#include "model/operation.hpp"
#include "model/parameter.hpp"
#include "model/primitive.hpp"
#include "model/struct.hpp"

using namespace std;
using namespace Api::Parser;


const char* LangConfigParser::KEY_TYPEMAP                   = "typemap";
const char* LangConfigParser::KEY_TYPE_PRIMITIVES           = "primitives";
const char* LangConfigParser::KEY_TYPE_CONTAINERS           = "containers";

const char* LangConfigParser::KEY_STYLE                     = "style";

const char* LangConfigParser::KEY_STYLE_NAMESTYLE           = "name-style";
const char* LangConfigParser::KEY_STYLE_NAMEDELIMITER       = "name-delimiter";
const char* LangConfigParser::KEY_STYLE_NAMEUSESHORT        = "name-use-short";
const char* LangConfigParser::KEY_STYLE_INDENT              = "indent";

const char  LangConfigParser::TYPE_PLACEHOLDER              = '@';

const char* LangConfigParser::styleContextKeys[] =
{
    "default", "class", "constant", "container", "doc", "enum", "event",
    "namespace", "operation", "parameter", "primitive", "struct", "type", "value"
};

const char* LangConfigParser::nameStyleKeys[] =
{
    "camelcase", "lowercase", "uppercase"
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


//string LangConfigParser::containerTypeToLang(Model::TypePtr type)
//{
//    if (Model::ResolvedTypePtr resolvedType = dynamic_pointer_cast<Model::ResolvedType>(type))
//    {
//        string output;

//        if (Model::ContainerPtr container = dynamic_pointer_cast<Model::Container>(resolvedType->primary()))
//        {
//            output = containerToLang(container);

//            vector<string> params;

//            // resolve subtype strings
//            for (Model::NamespaceMemberPtr param : resolvedType->params())
//            {
//                if (Model::PrimitivePtr primitive = dynamic_pointer_cast<Model::Primitive>(param))
//                {
//                    params.push_back(primitiveToLang(primitive));
//                }
//                else
//                {
//                    ///< @todo: how do we know if we need to add a namespace?
//                    params.push_back(param->longName());
//                }
//            }

//            // replace subtype placeholders
//            size_t pos = 0;
//            int paramCount = 0;

//            while ((pos = output.find(TYPE_PLACEHOLDER, pos)) != string::npos)
//            {
//                output.replace(pos, 1, params.at(paramCount));
//                ++paramCount;
//            }

//            if (paramCount != params.size())
//            {
//                throw runtime_error("LangConfigParser::containerTypeToLang(): subtype count in " +
//                                    resolvedType->primary()->longName() +
//                                    " definition does not match language-specific config!\n");
//            }
//            return output;
//        }
//        throw runtime_error("LangConfigParser::containerTypeToLang(): primary is not a container!\n");
//    }
//    throw runtime_error("LangConfigParser::containerTypeToLang(): bad type!\n");
//}


YAML::Node LangConfigParser::configValue(string key, Api::Model::IdentifiablePtr identifiable)
{
    return configValue(key, identifiableToStyleContext(identifiable));
}


YAML::Node LangConfigParser::configValue(string key, LangConfigParser::StyleContext styleContext)
{
    if (mRootNode[KEY_STYLE].IsDefined())
    {
        if (mRootNode[KEY_STYLE][styleContextKeys[styleContext]].IsDefined())
        {
            if (mRootNode[KEY_STYLE][styleContextKeys[styleContext]][key].IsScalar())
            {
                return mRootNode[KEY_STYLE][styleContextKeys[styleContext]][key];
            }
        }
        if (mRootNode[KEY_STYLE][styleContextKeys[StyleContext::DEFAULT]].IsDefined())
        {
            if (mRootNode[KEY_STYLE][styleContextKeys[StyleContext::DEFAULT]][key].IsScalar())
            {
                // no specialized config found - using default
                return mRootNode[KEY_STYLE][styleContextKeys[StyleContext::DEFAULT]][key];
            }
            throw runtime_error("key " + key + " neither found in style-context " + string(styleContextKeys[styleContext]) +
                                " nor in context " + string(styleContextKeys[StyleContext::DEFAULT]));
        }
        throw runtime_error("style context " + string(styleContextKeys[styleContext]) + " not found in style config");
    }
    throw runtime_error("section " + string(KEY_STYLE) + " not found in language config");
}


LangConfigParser::NameStyle LangConfigParser::configNameStyle(Api::Model::IdentifiablePtr identifiable)
{
    return configNameStyle(identifiableToStyleContext(identifiable));
}


LangConfigParser::NameStyle LangConfigParser::configNameStyle(LangConfigParser::StyleContext styleContext)
{
    const YAML::Node &node = configValue(KEY_STYLE_NAMESTYLE, styleContext);

    if (node.IsScalar())
    {
        for (int n = 0; n < NameStyle::_NAME_STYLE_COUNT_; ++n)
        {
            if (nameStyleKeys[n] == node.Scalar())
            {
                return (NameStyle) n;
            }
        }
    }
    return CAMELCASE;
}


bool LangConfigParser::configUseShortNames(StyleContext styleContext)
{
    const YAML::Node &node = configValue(KEY_STYLE_NAMEUSESHORT, styleContext);
    return node.as<bool>();
}


string LangConfigParser::configNameDelimiter(LangConfigParser::StyleContext styleContext)
{
    const YAML::Node &node = configValue(KEY_STYLE_NAMEDELIMITER, styleContext);
    return node.as<string>();
}


uint LangConfigParser::configIndent(LangConfigParser::StyleContext styleContext)
{
    const YAML::Node &node = configValue(KEY_STYLE_INDENT, styleContext);
    return node.as<uint>();
}


LangConfigParser::StyleContext LangConfigParser::identifiableToStyleContext(Api::Model::IdentifiablePtr identifiable)
{
    StyleContext context = DEFAULT;

    if (dynamic_pointer_cast<Model::Class>(identifiable))
    {
        context = CLASS;
    }
    else if (dynamic_pointer_cast<Model::Constant>(identifiable))
    {
        context = CONSTANT;
    }
    else if (dynamic_pointer_cast<Model::Container>(identifiable))
    {
        context = CONTAINER;
    }
    else if (dynamic_pointer_cast<Model::Documentation>(identifiable))
    {
        context = DOC;
    }
    else if (dynamic_pointer_cast<Model::Enum>(identifiable))
    {
        context = ENUM;
    }
    else if (dynamic_pointer_cast<Model::Event>(identifiable))
    {
        context = EVENT;
    }
    else if (dynamic_pointer_cast<Model::Namespace>(identifiable))
    {
        context = NAMESPACE;
    }
    else if (dynamic_pointer_cast<Model::Operation>(identifiable))
    {
        context = OPERATION;
    }
    else if (dynamic_pointer_cast<Model::Parameter>(identifiable))
    {
        context = PARAMETER;
    }
    else if (dynamic_pointer_cast<Model::Primitive>(identifiable))
    {
        context = PRIMITIVE;
    }
    else if (dynamic_pointer_cast<Model::Struct>(identifiable))
    {
        context = STRUCT;
    }
    return context;
}
