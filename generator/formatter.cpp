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


string Formatter::name(DomainObjectPtr domainObject)
{
    bool useShortName = mParser.configAttribute<bool>(LangConfigParser::StyleAttribute::NAME_USE_SHORT, domainObject);
    string name;

    if (IdentifiablePtr identifiable = dynamic_pointer_cast<Identifiable>(domainObject))
    {
        if (useShortName && identifiable->shortName().size())
        {
            name = identifiable->shortName();
        }
        else
        {
            name = identifiable->longName();
        }
        return styleToken(name, domainObject);
    }
    throw runtime_error("Formatter::name(): domainObject " + domainObject->objectTypeName() + "is not an Identifiable!");
}


string Formatter::type(TypePtr type, bool fullyQualified)
{
    if (ResolvedTypePtr resolvedType = dynamic_pointer_cast<ResolvedType>(type))
    {
        string token;

        if (PrimitivePtr primitive = dynamic_pointer_cast<Primitive>(resolvedType->primary()))
        {
            token = styleToken(mParser.primitiveToLang(primitive), primitive);
        }
        else if (dynamic_pointer_cast<Container>(resolvedType->primary()))
        {
            token = styleToken(containerTypeToLang(resolvedType, fullyQualified), resolvedType);
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
        output << indent() << " * @brief " << wrapText(doc->brief(), doc, " * ") << endl;

        if (doc->more().size())
        {
            output << indent() << " *" << endl;
            output << indent() << " * " + wrapText(doc->more(), doc, " * ") << endl;
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


void Formatter::beginIndent(DomainObjectPtr styleContextObject)
{
    beginIndent(mParser.configAttribute<uint>(LangConfigParser::StyleAttribute::INDENT, styleContextObject));
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


string Formatter::styleToken(string input, DomainObjectPtr styleContextObject)
{
    string output;

    if (styleContextObject->objectTypeName() != Primitive::TYPE_NAME &&
        styleContextObject->objectTypeName() != Container::TYPE_NAME)
    {
        LangConfigParser::NameStyle nameStyle = mParser.configNameStyle(styleContextObject);
        string delimiter = mParser.configAttribute<string>(LangConfigParser::StyleAttribute::NAME_DELIMITER, styleContextObject);

        boost::regex regularExpression("[A-Z][a-z]*|(?:::)");
        string::const_iterator start = input.begin();
        string::const_iterator end = input.end();
        boost::smatch matches;
        int elementCount = 0;

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

            if (elementCount)
            {
                temp = delimiter + temp;
            }

            output += temp;
            ++elementCount;

            // update search position:
            start = matches[0].second;
        }
    }
    else
    {
        output = input;
    }

    return output;
}


string Formatter::objectNamespace(DomainObjectPtr domainObject)
{
    DomainObjectPtr tempPtr = domainObject->parentObject();
    string namespaceName;
    string delimiter = mParser.configAttribute<string>(LangConfigParser::StyleAttribute::NAME_DELIMITER, domainObject);

    while (tempPtr)
    {
        if (IdentifiablePtr identifiable = dynamic_pointer_cast<Identifiable>(domainObject))
        {
            if (identifiable->longName() != "::")        ///< @todo: fix this root namespace mess
            {
                namespaceName = identifiable->longName() + delimiter + namespaceName;
            }
            tempPtr = tempPtr->parentObject();
        }
        else
        {
            throw runtime_error("objectNamespace(): object " + domainObject->objectTypeName() + " is no Identifiable!\n");
        }
    }
    return namespaceName;
}


string Formatter::wrapText(string text, DomainObjectPtr styleContextObject, string linePrefix)
{
    uint wrapLength = mParser.configAttribute<uint>(LangConfigParser::StyleAttribute::TEXT_WRAP, styleContextObject);

    size_t pos = wrapLength;
    char whitespace = ' ';

    while (pos < text.size())
    {
        size_t foundPos = text.rfind(whitespace, pos);

        if (foundPos != string::npos)
        {
            text.replace(foundPos, 1, "\n" + indent() + linePrefix);
            pos = foundPos + wrapLength + 1;
        }
        else
        {
            break;
        }
    }
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


