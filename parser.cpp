#include "parser.hpp"
#include <model/class.hpp>
#include <model/enum.hpp>
#include <model/struct.hpp>
#include <model/container.hpp>
#include <model/primitive.hpp>
#include <model/documentation.hpp>
#include <model/operation.hpp>
#include <model/event.hpp>
#include <model/parameter.hpp>
#include <model/type.hpp>
#include <model/value.hpp>


using namespace std;
using namespace Api;
using namespace Model;

const char *Parser::TYPE_NAMESPACE      = "namespace";
const char *Parser::TYPE_CLASS          = "class";
const char *Parser::TYPE_PRIMITIVE      = "primitive";
const char *Parser::TYPE_ENUM           = "enum";
const char *Parser::TYPE_STRUCT         = "struct";
const char *Parser::TYPE_CONTAINER      = "container";

const char *Parser::KEY_NAME            = "name";
const char *Parser::KEY_SHORTNAME       = "short";
const char *Parser::KEY_NODETYPE        = "nodetype";
const char *Parser::KEY_TYPE            = "type";
const char *Parser::KEY_VALUE           = "value";
const char *Parser::KEY_RETURN          = "return";

const char *Parser::KEY_DOC             = "doc";
const char *Parser::KEY_BRIEF           = "brief";
const char *Parser::KEY_MORE            = "more";

const char *Parser::KEY_MEMBERS         = "members";
const char *Parser::KEY_PARAMS          = "params";
const char *Parser::KEY_FIELDS          = "fields";
const char *Parser::KEY_VALUES          = "values";
const char *Parser::KEY_OPERATIONS      = "operations";
const char *Parser::KEY_OBSERVABLES     = "observables";

const char *Parser::FLAG_STATIC         = "static";
const char *Parser::FLAG_SYNCHRONOUS    = "synchronous";
const char *Parser::FLAG_VALUETYPE      = "valueType";
const char *Parser::FLAG_INHERITS       = "inherits";


Parser::Parser(Model::NamespacePtr rootNamespace) :
    mRootNamespace(rootNamespace)
{
    mParserMethods[TYPE_NAMESPACE]  = &Parser::parseNamespace;
    mParserMethods[TYPE_CLASS]      = &Parser::parseClass;
    mParserMethods[TYPE_PRIMITIVE]  = &Parser::parsePrimitive;
    mParserMethods[TYPE_ENUM]       = &Parser::parseEnum;
    mParserMethods[TYPE_STRUCT]     = &Parser::parseStruct;
    mParserMethods[TYPE_CONTAINER]  = &Parser::parseContainer;
}


void Api::Parser::parseFile(std::string filename)
{
    mYamlConfig = YAML::LoadFile(filename);

//    vector<string> nodeTypes;
//    extractNodeTypes(mYamlConfig, nodeTypes);

    parseNodeSequence(mYamlConfig, mRootNamespace);

    cout << "root namespace = " << mRootNamespace->longName() << endl;

//    for (auto nodeType : nodeTypes)
//    {
//        cout << "parsing " << nodeType << endl;
//    }

}


void Api::Parser::setRootNamespace(Api::Model::NamespacePtr rootNamespace)
{
    mRootNamespace = rootNamespace;
}


//void Parser::extractNodeTypes(const YAML::Node &node, std::vector<string> &nodeTypes)
//{
//    cout << "node size = " << node.size() << endl;
//    for (int n = 0; n < node.size(); ++n)
//    {
//        string s = node[n]["nodetype"].as<string>();
//        cout << "found nodetype " << s << " @ position " << n << endl;
//        nodeTypes.push_back(s);
//    }
//}


void Parser::parseNodeSequence(const YAML::Node &node, NamespacePtr rootNamespace)
{
    static int counter = 0;
    ++counter;

    for (int n = 0; n < node.size(); ++n)
    {
        if (node[n][KEY_NODETYPE].IsScalar())
        {
            string nodeType = node[n][KEY_NODETYPE].as<string>();
            cout << "found nodetype " << nodeType << " on level " << counter << endl;

            try {
                MemberFunc func = mParserMethods[nodeType];
                NamespaceMemberPtr member = (this->*func)(node[n]);
                rootNamespace->addMember(member);
                cout << "got new namespace member " << member->longName() << " on level " << counter << endl;
            }
            catch (exception e)
            {
                cout << "PARSER ERROR: nodetype " << nodeType << " unknown! (" << e.what() << ")" << endl;
            }
        }
        else
        {
            cout << "oh oh, no nodetype defined!" << endl;
        }
    }

    --counter;
}


NamespaceMemberPtr Parser::parseNamespace(const YAML::Node &node)
{
    NamespacePtr newNamespace = newNamespaceMember<Namespace>(node);

    cout << "parsing namespace " << newNamespace->longName() << endl;

    parseNodeSequence(node[KEY_MEMBERS], newNamespace);

    cout << "new namespace " << newNamespace->longName() << endl;
    return newNamespace;
}


NamespaceMemberPtr Parser::parseClass(const YAML::Node &node)
{
    cout << "parseClass()" << endl;
    ClassPtr newClass = newNamespaceMember<Class>(node);
    return newClass;
}


NamespaceMemberPtr Parser::parsePrimitive(const YAML::Node &node)
{
    cout << "parsePrimitive()" << endl;

    PrimitivePtr newPrimitive = newNamespaceMember<Primitive>(node);

    return newPrimitive;
}

NamespaceMemberPtr Parser::parseEnum(const YAML::Node &node)
{
    cout << "parseEnum()" << endl;
    EnumPtr newEnum = newNamespaceMember<Enum>(node);
    return newEnum;
}

NamespaceMemberPtr Parser::parseStruct(const YAML::Node &node)
{
    cout << "parseStruct()" << endl;
    StructPtr newStruct = newNamespaceMember<Struct>(node);
    return newStruct;
}

NamespaceMemberPtr Parser::parseContainer(const YAML::Node &node)
{
    cout << "parseContainer()" << endl;
    ContainerPtr newContainer = newNamespaceMember<Container>(node);
    return newContainer;
}

void Parser::parseName(const YAML::Node &node, IdentifiablePtr identifiable)
{
    if (node[KEY_NAME].IsScalar())
    {
        identifiable->setLongName(node[KEY_NAME].Scalar());

        if (node[KEY_SHORTNAME].IsScalar())
        {
            identifiable->setLongName(node[KEY_SHORTNAME].Scalar());
        }
    }
    else
    {
        cout << "argh, no name" << endl;
        throw exception();
    }
}

void Parser::parseDoc(const YAML::Node &node, IdentifiablePtr identifiable)
{
    const YAML::Node &docNode = node[KEY_DOC];

    if (docNode.IsMap())
    {
        DocumentationPtr newDoc = std::shared_ptr<Documentation>(new Documentation);

        if (docNode[KEY_BRIEF].IsScalar())
        {
            newDoc->setBrief(docNode[KEY_BRIEF].Scalar());

            if (docNode[KEY_BRIEF].IsScalar())
            {
                newDoc->setBrief(docNode[KEY_BRIEF].Scalar());
            }
            identifiable->setDoc(newDoc);
        }
        else
        {
            cout << "documentation of " << identifiable->longName() << " is missing brief description" << endl;
        }
    }
    else
    {
        cout << "doc-node of " << identifiable->longName() << " is not set" << endl;
    }
}

template <class T>
std::shared_ptr<T> Parser::newNamespaceMember(const YAML::Node &node)
{
    std::shared_ptr<T> newMember(new T);

    parseName(node, newMember);
    parseDoc(node, newMember);
    return newMember;
}


//void Api::Parser::parseNode(const YAML::Node &node)
//{
//    static int counter = 0;

//    ++counter;
//    std::string padding(counter * 2, '.');

//    if (node.IsMap())
//    {
//        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
//        {
//            const YAML::Node &key = it->first;
//            const YAML::Node &value = it->second;

//            if (key.IsScalar())
//            {
//                cout << padding << key.Scalar() << ": ";
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
//                cout << padding << "*** KEY TYPE: " << key.Type() << endl;
//            }

//            if (value.IsMap() || value.IsSequence())
//            {
//                parseNode(value);
//            }
//        }
//    }
//    else if (node.IsSequence())
//    {
//        for (int n = 0; n < node.size(); ++n)
//        {
////            cout << padding << "parsing sequence node " << n << endl;
//            cout << endl;
//            parseNode(node[n]);
//        }
//    }
//    else
//    {
//        cout << padding << "strange node " << node.Scalar() << endl;
//    }

//    --counter;
//}



