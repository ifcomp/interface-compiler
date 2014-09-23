#include "formatter.hpp"

#include "model/resolvedType.hpp"
#include "model/container.hpp"
#include "model/primitive.hpp"
#include "parser/langConfigParser.hpp"

#include <boost/regex.hpp>
#include <sstream>

using namespace Api::Gen;
using namespace Api::Model;
using namespace Api::Parser;
using namespace std;


Formatter::Formatter(std::string configFilename)
    : mParser(configFilename)
    , mCurrentIndent(0)
{
    cout << "starting to parse configfile " << configFilename << endl;
    mParser.parseTypeMap();
}


string Formatter::name(IdentifiablePtr identifiable)
{
    bool useShortName = mParser.configUseShortNames(mParser.identifiableToStyleContext(identifiable));
    string name;

    if (useShortName && identifiable->shortName().size())
    {
        name = identifiable->shortName();
    }
    else
    {
        name = identifiable->longName();
    }
    return styleToken(name, mParser.identifiableToStyleContext(identifiable));
}


string Formatter::type(TypePtr type, bool fullyQualified)
{
    if (ResolvedTypePtr resolvedType = dynamic_pointer_cast<ResolvedType>(type))
    {
        string token;

        if (PrimitivePtr primitive = dynamic_pointer_cast<Primitive>(resolvedType->primary()))
        {
            token = styleToken(mParser.primitiveToLang(primitive), LangConfigParser::StyleContext::PRIMITIVE);
        }
        else if (dynamic_pointer_cast<Container>(resolvedType->primary()))
        {
            token = styleToken(containerTypeToLang(resolvedType, fullyQualified), LangConfigParser::StyleContext::CONTAINER);
        }
        else
        {
            token = name(resolvedType->primary());

            if (fullyQualified)
            {
                token = objectNamespace(resolvedType->primary()) + token;
            }
        }

        return token;
    }
    throw runtime_error("Formatter::type() : type object is not of ResolvedType\n");
}


string Formatter::doc(DocumentationPtr doc)
{
    stringstream output;

    if (doc)
    {
        output << indent() << endl;
        output << indent() << "/**" << endl;
        output << indent() << " * @brief " << wrapText(doc->brief()) << endl;

        if (doc->more().size())
        {
            output << indent() << " *" << endl;
            output << indent() << " * " + wrapText(doc->more()) << endl;
        }

        output << indent() << " */" << endl;
    }
    return output.str();
}



void Formatter::beginIndent(uint32_t indent)
{
    mIndentStack.push(indent);
    mCurrentIndent += indent;
}


void Formatter::beginIndent(IdentifiablePtr identifiable)
{
    ///< @todo: fetch indent from config
    beginIndent(4);
}


void Formatter::endIndent()
{
    mCurrentIndent -= mIndentStack.top();
    mIndentStack.pop();
}


uint32_t Formatter::indentCount()
{
    return mCurrentIndent;
}


string Formatter::indent()
{
    return string(mCurrentIndent, ' ');
}


string Formatter::styleToken(string input, LangConfigParser::StyleContext styleContext)
{
    string output = "~" + std::to_string(styleContext) + "~";

    if (styleContext != LangConfigParser::StyleContext::PRIMITIVE &&
        styleContext != LangConfigParser::StyleContext::CONTAINER)
    {
        LangConfigParser::NameStyle nameStyle = mParser.configNameStyle(styleContext);

        boost::regex regularExpression("[A-Z][a-z]*|(?:::)");
        string::const_iterator start    = input.begin();
        string::const_iterator end      = input.end();
        boost::smatch matches;

        while(regex_search(start, end, matches, regularExpression))
        {
            string temp(matches[0]);

            switch (nameStyle)
            {
                case LangConfigParser::NameStyle::UPPERCASE:
                    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                    break;

                case LangConfigParser::NameStyle::LOWERCASE:
                    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                    break;

                default:
                    // no transform needed for CamelCase
                    break;
            }

            output += temp;

            // update search position:
            start = matches[0].second;
        }
    }
    else
    {
        output = "PRIM:" + input;
    }

    return output + "|";
}


string Formatter::objectNamespace(IdentifiablePtr identifiable)
{
    IdentifiablePtr tempPtr = identifiable->parentIdentifiable();
    string namespaceName;
    string delimiter = "::";            ///< @todo: retrieve delimiter from parser

    while (tempPtr)
    {
        if (tempPtr->longName() != "::")        ///< @todo: fix this root namespace mess
        {
            namespaceName = tempPtr->longName() + delimiter + namespaceName;
        }
        tempPtr = tempPtr->parentIdentifiable();
    }
    return namespaceName;
}


string Formatter::wrapText(string text, IdentifiablePtr identifiable)
{
    int wrapLength = 80;        ///< @todo: fetch max-length from config

//    size_t pos = 0;
//    while (pos < (text.length() - wrapLength))
//    {

//    }
    return text;
}


string Formatter::containerTypeToLang(Model::TypePtr type, bool fullyQualified)
{
    if (Model::ResolvedTypePtr resolvedType = dynamic_pointer_cast<Model::ResolvedType>(type))
    {
        string output;

        if (Model::ContainerPtr container = dynamic_pointer_cast<Model::Container>(resolvedType->primary()))
        {
            output = mParser.containerToLang(container);

            vector<string> params;

            // resolve subtype strings
            for (Model::NamespaceMemberPtr param : resolvedType->params())
            {
                if (Model::PrimitivePtr primitive = dynamic_pointer_cast<Model::Primitive>(param))
                {
                    // primitives are directly translated to string from language config
                    params.push_back(mParser.primitiveToLang(primitive));
                }
                else if (dynamic_pointer_cast<Model::Container>(param))
                {
                    throw runtime_error("Sorry, nested containers art not supported!\n");
                }
                else
                {
                    if (fullyQualified)
                    {
                        params.push_back(objectNamespace(param) + name(param));
                    }
                    else
                    {
                        params.push_back(name(param));
                    }
                }
            }

            // replace subtype placeholders
            size_t pos = 0;
            int paramCount = 0;

            while ((pos = output.find(LangConfigParser::TYPE_PLACEHOLDER, pos)) != string::npos)
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


