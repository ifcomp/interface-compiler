#include "formatter.hpp"

#include "model/resolvedType.hpp"
#include "model/container.hpp"
#include "model/primitive.hpp"

#include <sstream>

using namespace Api::Gen;
using namespace Api::Model;
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
    ///< @todo: check short name config in parser
    return styleToken(identifiable->longName());
}


string Formatter::type(TypePtr type, bool fullyQualified)
{
    if (ResolvedTypePtr resolvedType = dynamic_pointer_cast<ResolvedType>(type))
    {
        string token;

        if (PrimitivePtr primitive = dynamic_pointer_cast<Primitive>(resolvedType->primary()))
        {
            token = mParser.primitiveToLang(primitive);
        }
        else if (dynamic_pointer_cast<Container>(resolvedType->primary()))
        {
            token = mParser.containerTypeToLang(resolvedType);
        }
        else
        {
            token = resolvedType->primary()->longName();

            if (fullyQualified)
            {
                token = objectNamespace(resolvedType->primary()) + token;
            }
        }

        token = styleToken(token);

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


string Formatter::styleToken(string name, IdentifiablePtr identifiable)
{
    ///< @todo: lookup style in parser
    return name;
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

