#include "Components/LangConfigReader.hpp"

#include "Model/Class.hpp"
#include "Model/Class/Constant.hpp"
#include "Model/Documentation.hpp"
#include "Model/Enum.hpp"
#include "Model/Class/Event.hpp"
#include "Model/Namespace.hpp"
#include "Model/Class/Operation.hpp"
#include "Model/Parameter.hpp"
#include "Model/Primitive.hpp"
#include "Model/Struct.hpp"

#include <boost/regex.hpp>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;


namespace Everbase { namespace InterfaceCompiler { namespace Components {


const char* LangConfigReader::KEY_SECTION_TYPEMAP       = "typemap";
const char* LangConfigReader::KEY_TYPE_PRIMITIVES       = "primitives";
const char* LangConfigReader::KEY_TYPE_CONTAINERS       = "containers";
const char* LangConfigReader::KEY_SECTION_STYLE         = "style";
const char* LangConfigReader::KEY_STYLE_CONTEXT_DEFAULT = "default";

const char  LangConfigReader::TYPE_PLACEHOLDER          = '@';


const char* LangConfigReader::styleAttributeKeys[] =
{
    "name-style", "name-delimiter", "name-use-short", "namespace-delimiter", "text-wrap"
};


const char* LangConfigReader::nameStyleKeys[] =
{
    "lower-camelcase", "upper-camelcase", "lowercase", "uppercase"
};


LangConfigReader::LangConfigReader(std::istream& stream)
{
    mRootNode = loadFile(stream);
}


void LangConfigReader::parseTypeMap()
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
        /*
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
        */
    }
    cout << mPrimitiveMap.size() << " primitives, " << /*mContainerMap.size() <<*/ " containers" << endl;
}


std::string LangConfigReader::primitiveToLang(Model::PrimitiveRef primitive) const
{
    string typeName = primitive->underlyingName();

    if (mPrimitiveMap.find(typeName) == mPrimitiveMap.end())
    {
        throw runtime_error("primitive type " + typeName + " not declared in language config!\n");
    }

    return mPrimitiveMap.at(typeName);
}

/*
string LangConfigReader::containerToLang(Model::ContainerRef container) const
{
    string typeName = container->typeName();

    if (mContainerMap.find(typeName) == mContainerMap.end())
    {
        throw runtime_error("container type " + typeName + " not declared in language config!\n");
    }
    return mContainerMap.at(typeName);
}


string LangConfigReader::containerTypeToLang(Model::TypeBaseRef type, bool fullyQualified) const
{
    if (Model::TypeRef resolvedType = dynamic_pointer_cast<Model::Type>(type))
    {
        string output;

        if (Model::ContainerRef container = dynamic_pointer_cast<Model::Container>(resolvedType->primary()))
        {
            output = containerToLang(container);

            vector<string> params;

            // resolve subtype strings
            for (Model::NamespaceMemberRef param : resolvedType->params())
            {
                if (Model::PrimitiveRef primitive = dynamic_pointer_cast<Model::Primitive>(param))
                {
                    // primitives are directly translated to string from language config
                    params.push_back(primitiveToLang(primitive));
                }
                else if (dynamic_pointer_cast<Model::Container>(param))
                {
                    throw runtime_error("Sorry, nested containers art not supported!\n");
                }
                else
                {
                    params.push_back((fullyQualified ? formatNamespace(param) : "") + styleToken(param->longName(), param));
                }
            }

            // replace subtype placeholders
            size_t pos = 0;
            size_t paramCount = 0;

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
*/

string LangConfigReader::formatNamespace(Model::IdentifiableRef identifiable) const
{
    using namespace Model;

    NameStyle nameStyle;
    string namespaceName;
    string delimiter = configAttribute<string>(LangConfigReader::StyleAttribute::NAMESPACE_DELIMITER, identifiable);

    IdentifiableRef parentRef = dynamic_pointer_cast<Identifiable>(identifiable->parent());

    while (parentRef && parentRef->longName() != "::")
    {
		string nameDelimiter = configAttribute<string>(LangConfigReader::StyleAttribute::NAME_DELIMITER, parentRef);
        nameStyle = configNameStyle(parentRef, StyleAttribute::NAME_STYLE);
        namespaceName = styleToken(parentRef->longName(), nameStyle, nameDelimiter) + delimiter + namespaceName;
        parentRef = dynamic_pointer_cast<Identifiable>(parentRef->parent());
    }
    return namespaceName;
}


LangConfigReader::NameStyle LangConfigReader::configNameStyle(Model::ObjectRef styleContextObject, StyleAttribute styleAttribute) const
{
    const YAML::Node &node = configValue(styleAttribute, styleContextObject);

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
    throw runtime_error("name style " + node.Scalar() + " not supported");
}


YAML::Node LangConfigReader::configValue(LangConfigReader::StyleAttribute styleAttribute, Model::ObjectRef styleContextObject) const
{
    string styleAttributekey(styleAttributeKeys[styleAttribute]);
    string styleContextKey = "default";

    if (styleContextObject)
    {
        styleContextKey = styleContextObject->typeName();
        transform(styleContextKey.begin(), styleContextKey.end(), styleContextKey.begin(), ::tolower);
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
        if (mRootNode[KEY_SECTION_STYLE][LangConfigReader::KEY_STYLE_CONTEXT_DEFAULT].IsDefined())
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


std::string LangConfigReader::listKnownStyleAttributes() const
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


string LangConfigReader::styleToken(string input, Model::ObjectRef styleContextObject) const
{
    if (styleContextObject->typeName() != Model::Primitive::TYPE_NAME /*&&
        styleContextObject->typeName() != Model::Container::TYPE_NAME*/)
    {
        NameStyle nameStyle = configNameStyle(styleContextObject);
        string delimiter = configAttribute<string>(StyleAttribute::NAME_DELIMITER, styleContextObject);

        return styleToken(input, nameStyle, delimiter);
    }
    return input;
}


string LangConfigReader::styleToken(string input, LangConfigReader::NameStyle nameStyle, string delimiter) const
{
    boost::regex regularExpression("[A-Z]*[a-z]+|[A-Z]*[0-9]+|[A-Z]+$");
	
    string::const_iterator start = input.begin();
    string::const_iterator end = input.end();
    boost::smatch matches;
    int elementCount = 0;
    string output;

    while(regex_search(start, end, matches, regularExpression))
    {
        string temp(matches[0]);

        switch (nameStyle)
        {
            case NameStyle::LOWER_CAMELCASE:
                transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

                if (elementCount > 0)
                {
                    // only first word starts lowercase
                    transform(temp.begin(), temp.begin() + 1, temp.begin(), ::toupper);
                }
                break;

            case NameStyle::UPPER_CAMELCASE:
                transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                transform(temp.begin(), temp.begin() + 1, temp.begin(), ::toupper);
                break;

            case NameStyle::UPPERCASE:
                transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                break;

            case NameStyle::LOWERCASE:
                transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                break;

            case NameStyle::_NAME_STYLE_COUNT_:
            default:
                throw std::runtime_error("invalid name style");
        }

        if (elementCount)
        {
            temp = delimiter + temp;
        }

        output += temp;
        ++elementCount;

        // update search position:
        start = matches[0].second;
    }
    return output;
}


} } } // namespace: Everbase::InterfaceCompiler::Components
