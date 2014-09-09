#include "parser.hpp"


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
const char *Parser::KEY_EVENTS          = "observables";

const char *Parser::FLAG_STATIC         = "static";
const char *Parser::FLAG_SYNCHRONOUS    = "synchronous";
const char *Parser::FLAG_VALUETYPE      = "valueType";
const char *Parser::KEY_INHERITS        = "inherits";


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

    parseNodeSequence(mYamlConfig, mRootNamespace);

    cout << "root namespace = " << mRootNamespace->longName() << endl;

    for (auto member : mRootNamespace->members())
    {
        cout << "MEMBER: " << member.first << endl;
        NamespacePtr namespacePtr = dynamic_pointer_cast<Namespace>(member.second);

        if (namespacePtr)
        {
            for (auto member : namespacePtr->members())
            {
                cout << "2. MEMBER: " << member.first << endl;
            }
        }
    }
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

            try
            {
                MemberFunc func = mParserMethods[nodeType];
                NamespaceMemberPtr member = (this->*func)(node[n]);
                rootNamespace->addMember(member);
                cout << "new namespace member " << member->longName() << " on level " << counter << endl;
            }
            catch (const exception &e)
            {
                cout << "PARSER ERROR: nodetype " << nodeType << " unknown! (" << e.what() << ")" << endl;
            }
        }
        else
        {
            cout << "PARSER ERROR: no nodetype defined!" << endl;
        }
    }

    --counter;
}


NamespaceMemberPtr Parser::parseNamespace(const YAML::Node &node)
{
    NamespacePtr newNamespace = newIdentifiable<Namespace>(node);

    cout << "parsing namespace " << newNamespace->longName() << endl;

    parseNodeSequence(node[KEY_MEMBERS], newNamespace);

    cout << "new namespace " << newNamespace->longName() << endl;
    return newNamespace;
}


NamespaceMemberPtr Parser::parseClass(const YAML::Node &node)
{
    cout << "parseClass()" << endl;
    ClassPtr newClass = newIdentifiable<Class>(node);

    if (node[FLAG_VALUETYPE].IsScalar() && node[FLAG_VALUETYPE].as<bool>())
    {
        newClass->setType(Class::ClassType::VALUE);
    }
    else
    {
        newClass->setType(Class::ClassType::ABSTRACT);
    }

    if (node[KEY_INHERITS].IsScalar())
    {
        cout << newClass->longName() << " inherits from " << node[KEY_INHERITS].Scalar();
        ///< TODO: resolve class
    }

    const YAML::Node &operationsNode = node[KEY_OPERATIONS];
    if (operationsNode.IsSequence())
    {
        for (int n = 0; n < operationsNode.size(); ++n)
        {
            newClass->addOperation(parseOperation(operationsNode[n]));
        }
    }

    const YAML::Node &eventsNode = node[KEY_EVENTS];
    if (eventsNode.IsSequence())
    {
        for (int n = 0; n < eventsNode.size(); ++n)
        {
            newClass->addEvent(parseEvent(eventsNode[n]));
        }
    }

    return newClass;
}


NamespaceMemberPtr Parser::parsePrimitive(const YAML::Node &node)
{
    cout << "parsePrimitive()" << endl;

    PrimitivePtr newPrimitive = newIdentifiable<Primitive>(node);

    return newPrimitive;
}


NamespaceMemberPtr Parser::parseEnum(const YAML::Node &node)
{
    cout << "parseEnum()" << endl;
    EnumPtr newEnum = newIdentifiable<Enum>(node);
    return newEnum;
}


NamespaceMemberPtr Parser::parseStruct(const YAML::Node &node)
{
    cout << "parseStruct()" << endl;
    StructPtr newStruct = newIdentifiable<Struct>(node);
    return newStruct;
}


NamespaceMemberPtr Parser::parseContainer(const YAML::Node &node)
{
    cout << "parseContainer()" << endl;
    ContainerPtr newContainer = newIdentifiable<Container>(node);
    return newContainer;
}


OperationPtr Parser::parseOperation(const YAML::Node &node)
{
    cout << "parseOperations()" << endl;
    OperationPtr newOperation = newIdentifiable<Operation>(node);
    return newOperation;
}


EventPtr Parser::parseEvent(const YAML::Node &node)
{
    cout << "parseEvent()" << endl;
    EventPtr newEvent = newIdentifiable<Event>(node);
    return newEvent;
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
std::shared_ptr<T> Parser::newIdentifiable(const YAML::Node &node)
{
    std::shared_ptr<T> newMember(new T);

    IdentifiablePtr identifiable = dynamic_pointer_cast<Identifiable>(newMember);
    if (identifiable)
    {
        parseName(node, identifiable);
        parseDoc(node, identifiable);
    }
    else
    {
        cout << "newIdentifiable(): Type is not an Identifiable!" << endl;
        throw exception();
    }
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



