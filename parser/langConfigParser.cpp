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

#include <sstream>

using namespace std;
using namespace Api::Parser;


const char* LangConfigParser::KEY_SECTION_TYPEMAP       = "typemap";
const char* LangConfigParser::KEY_TYPE_PRIMITIVES       = "primitives";
const char* LangConfigParser::KEY_TYPE_CONTAINERS       = "containers";
const char* LangConfigParser::KEY_SECTION_STYLE         = "style";
const char* LangConfigParser::KEY_STYLE_CONTEXT_DEFAULT = "default";

const char  LangConfigParser::TYPE_PLACEHOLDER          = '@';


const char* LangConfigParser::styleAttributeKeys[] =
{
    "name-style", "name-delimiter", "name-use-short", "indent", "text-wrap"
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
    if (checkNode(mRootNode, KEY_SECTION_TYPEMAP, YAML::NodeType::Map, true))
    {
        if (checkNode(mRootNode[KEY_SECTION_TYPEMAP], KEY_TYPE_PRIMITIVES, YAML::NodeType::Sequence, true))
        {
            for (auto primitiveNode : mRootNode[KEY_SECTION_TYPEMAP][KEY_TYPE_PRIMITIVES])
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
        if (checkNode(mRootNode[KEY_SECTION_TYPEMAP], KEY_TYPE_CONTAINERS, YAML::NodeType::Sequence, true))
        {
            for (auto containerNode : mRootNode[KEY_SECTION_TYPEMAP][KEY_TYPE_CONTAINERS])
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


LangConfigParser::NameStyle LangConfigParser::configNameStyle(DomainObjectPtr styleContextObject)
{
    const YAML::Node &node = configValue(StyleAttribute::NAME_STYLE, styleContextObject);

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


YAML::Node LangConfigParser::configValue(LangConfigParser::StyleAttribute styleAttribute, DomainObjectPtr styleContextObject)
{
    string styleAttributekey(styleAttributeKeys[styleAttribute]);
    string styleContextKey = "default";

    if (styleContextObject)
    {
        styleContextKey = styleContextObject->objectTypeName();
    }

    if (mRootNode[KEY_SECTION_STYLE].IsDefined())
    {
        if (mRootNode[KEY_SECTION_STYLE][styleContextKey].IsDefined())
        {
            if (mRootNode[KEY_SECTION_STYLE][styleContextKey][styleAttributekey].IsScalar())
            {
                return mRootNode[KEY_SECTION_STYLE][styleContextKey][styleAttributekey];
            }
        }
        if (mRootNode[KEY_SECTION_STYLE][LangConfigParser::KEY_STYLE_CONTEXT_DEFAULT].IsDefined())
        {
            if (mRootNode[KEY_SECTION_STYLE][KEY_STYLE_CONTEXT_DEFAULT][styleAttributekey].IsScalar())
            {
                // no specialized config found - using default
                return mRootNode[KEY_SECTION_STYLE][KEY_STYLE_CONTEXT_DEFAULT][styleAttributekey];
            }
        }
        throw runtime_error("key " + styleAttributekey + " neither found in style-context " + styleContextKey +
                            " nor in context " + KEY_STYLE_CONTEXT_DEFAULT + "\n\n" +
                            listKnownStyleAttributes());
    }
    throw runtime_error("section " + string(KEY_SECTION_STYLE) + " not found in language config");
}


std::string LangConfigParser::listKnownStyleAttributes()
{
    stringstream out;
    out << "-- REGISTERED STYLE ATTRIBUTES -" << endl;

    for (int n = 0; n < _STYLE_ATTRIB_COUNT_; ++n)
    {
        out << styleAttributeKeys[n] << endl;
    }

    out << "--------------------------------" << endl;
    return out.str();
}
