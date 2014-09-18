#include "formatter.hpp"

#include "model/resolvedType.hpp"
#include "model/container.hpp"
#include "model/primitive.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace std;


Formatter::Formatter(std::string configFilename)
    : mParser(configFilename)
{
    cout << "starting to parse configfile " << configFilename << endl;
    mParser.parseTypeMap();

//    if (sequenceNode.IsMap())
//    {
//        for (YAML::const_iterator it = sequenceNode.begin(); it != sequenceNode.end(); ++it)
//        {
//            const YAML::Node &key = it->first;
//            const YAML::Node &value = it->second;

//            if (key.IsScalar())
//            {
//                cout << key.Scalar() << ": ";
//                if (value.IsScalar())
//                {
//                    cout << value.Scalar();
//                }
//                else
//                {
//                    cout << " <value type = " << value.Type() << ">";
//                }
//                cout << endl;
//            }
//            else
//            {
//                cout << "*** KEY TYPE: " << key.Type() << endl;
//            }
//        }
//    }

}

std::string Formatter::name(IdentifiablePtr identifiable)
{
    ///< @todo: check short name config in parser
    return styleToken(identifiable->longName());
}


std::string Formatter::type(TypePtr type, bool fullyQualified)
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
        }

        token = styleToken(token);

        if (fullyQualified)
        {
            token = objectNamespace(resolvedType->primary()) + token;
        }
        return token;
    }
    throw runtime_error("Formatter::type() : type object is not of ResolvedType\n");
}


std::string Formatter::classType(ClassPtr classPtr)
{
    return "";
}


std::string Formatter::doc(Documentation doc)
{
    return "";
}

string Formatter::styleToken(string name, IdentifiablePtr identifiable)
{
    ///< @todo: lookup style in parser
    return name;
}

string Formatter::objectNamespace(IdentifiablePtr identifiable)
{
    IdentifiablePtr tempPtr = identifiable;
    string namespaceName;
    string delimiter = "::";            ///< todo: retrieve delimiter from parser

    while (tempPtr)
    {
        namespaceName += tempPtr->longName() + delimiter;
    }
    return namespaceName;
}

