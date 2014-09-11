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

const char *Parser::CONTAINER_VECTOR    = "vector";
const char *Parser::CONTAINER_LIST      = "list";
const char *Parser::CONTAINER_SET       = "set";

const char *Parser::KEY_NAME            = "name";
const char *Parser::KEY_SHORTNAME       = "short";
const char *Parser::KEY_NODETYPE        = "nodetype";
const char *Parser::KEY_CONTAINER_TYPE  = "containertype";
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
const char *Parser::KEY_EVENTS          = "events";

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

    try {
        parseNamespaceMembers(mYamlConfig, mRootNamespace);

        cout << "----------------------- TYPES ------------------------" << endl;
        for (auto type : mKnownTypes)
        {
            cout << type.first << endl;
        }

        cout << "----------------------- RESOLVE ------------------------" << endl;

        resolveTypesInNamespace(mRootNamespace);
    }
    catch (const std::exception e)
    {
        cout << "PARSE ERROR - please check your config" << endl;
    }

    cout << "root namespace = " << mRootNamespace->longName() << endl;

}


void Api::Parser::setRootNamespace(Api::Model::NamespacePtr rootNamespace)
{
    mRootNamespace = rootNamespace;
}


void Parser::parseNamespaceMembers(const YAML::Node &node, NamespacePtr rootNamespace)
{
    for (auto sequenceNode : node)
    {
        if (checkNode(sequenceNode, KEY_NODETYPE, YAML::NodeType::Scalar, true))
        {
            string nodeType = sequenceNode[KEY_NODETYPE].as<string>();
            cout << "found nodetype " << nodeType << endl;

            if (mParserMethods.find(nodeType) != mParserMethods.end())
            {
                MemberFunc func = mParserMethods[nodeType];
                NamespaceMemberPtr member = (this->*func)(sequenceNode);
                rootNamespace->addMember(member);
                cout << "new namespace member " << member->longName() << endl;
            }
            else
            {
                cout << "WARNING: ignoring unknown nodetype " << nodeType << endl;
            }
        }
    }
}


NamespaceMemberPtr Parser::parseNamespace(const YAML::Node &node)
{
    string namespaceName;

    if (checkNode(node, KEY_NAME))
    {
        namespaceName = node[KEY_NAME].Scalar();

        if (namespaceName.find("::") != string::npos)
        {
            cout << "ERROR: please add a namespace section for each namespace part in " << namespaceName << endl;
            throw std::exception();
        }
    }

    NamespacePtr newNamespace = dynamic_pointer_cast<Namespace>(resolveTypeName(namespaceName));

    if (newNamespace)
    {
        cout << " *** using existing namespace " << newNamespace->longName() << endl;
    }
    else
    {
        // Namespace doesn't exist --> create new namespace object
        newNamespace = newIdentifiable<Namespace>(node);
        registerType(newNamespace);
    }

    startNamespace(newNamespace);

    cout << "parsing namespace " << newNamespace->longName() << endl;

    if (checkNode(node, KEY_MEMBERS, YAML::NodeType::Sequence, true))
    {
        parseNamespaceMembers(node[KEY_MEMBERS], newNamespace);
    }

    endNamespace();
    return newNamespace;
}


NamespaceMemberPtr Parser::parseClass(const YAML::Node &node)
{
    cout << "parseClass()" << endl;
    ClassPtr newClass = newIdentifiable<Class>(node);
    registerType(newClass);

    if (node[FLAG_VALUETYPE].IsScalar() && node[FLAG_VALUETYPE].as<bool>())
    {
        newClass->setType(Class::ClassType::VALUE);
    }
    else
    {
        newClass->setType(Class::ClassType::ABSTRACT);
    }

    if (checkNode(node, KEY_INHERITS))
    {
        UnresolvedTypePtr newType = make_shared<UnresolvedType>();
        newType->setPrimary(node[KEY_INHERITS].Scalar());
        /// @TODO: newType should actually be a pointer to a class - this would only work after resolving
//        newClass->setParent(newType);
    }

    if (checkNode(node, KEY_OPERATIONS, YAML::NodeType::Sequence))
    {
        for (auto operationNode : node[KEY_OPERATIONS])
        {
            newClass->addOperation(parseOperation(operationNode));
        }
    }

    if (checkNode(node, KEY_EVENTS, YAML::NodeType::Sequence))
    {
        for (auto eventNode : node[KEY_EVENTS])
        {
            newClass->addEvent(parseEvent(eventNode));
        }
    }

    if (newClass->operations().size() == 0 &&
        newClass->events().size() == 0)
    {
        cout << "WARNING: no operations or events defined in class " << newClass->longName() << endl;
    }

    return newClass;
}


NamespaceMemberPtr Parser::parsePrimitive(const YAML::Node &node)
{
    // Primitives only consist of their names
    PrimitivePtr newPrimitive = newIdentifiable<Primitive>(node);
    registerType(newPrimitive);
    return newPrimitive;
}


NamespaceMemberPtr Parser::parseEnum(const YAML::Node &node)
{
    cout << "parseEnum()" << endl;
    EnumPtr newEnum = newIdentifiable<Enum>(node);
    registerType(newEnum);

    if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
    {
        for (auto enumNode : node[KEY_VALUES])
        {
            newEnum->addValue(parseValue(enumNode));
            cout << "enum value " << enumNode[KEY_NAME] << endl;
        }
    }

    return newEnum;
}


NamespaceMemberPtr Parser::parseStruct(const YAML::Node &node)
{
    StructPtr newStruct = newIdentifiable<Struct>(node);
    registerType(newStruct);

    if (checkNode(node, KEY_FIELDS, YAML::NodeType::Sequence, true))
    {
        for (auto fieldNode : node[KEY_FIELDS])
        {
            newStruct->addField(parseParameter(fieldNode));
        }
    }

    return newStruct;
}


NamespaceMemberPtr Parser::parseContainer(const YAML::Node &node)
{
    ContainerPtr newContainer = newIdentifiable<Container>(node);
    registerType(newContainer);

    if (checkNode(node, KEY_CONTAINER_TYPE, YAML::NodeType::Scalar, true))
    {
        string typeName = node[KEY_CONTAINER_TYPE].Scalar();
        Container::ContainerType type;

        if (typeName == CONTAINER_VECTOR)
        {
            type = Container::ContainerType::VECTOR;
        }
        else if (typeName == CONTAINER_LIST)
        {
            type = Container::ContainerType::LIST;
        }
        else if (typeName == CONTAINER_SET)
        {
            type = Container::ContainerType::SET;
        }

        newContainer->setType(type);
    }

    return newContainer;
}


OperationPtr Parser::parseOperation(const YAML::Node &node)
{
    OperationPtr newOperation = newIdentifiable<Operation>(node);

    if (checkNode(node, FLAG_STATIC))
    {
        newOperation->setStatic(node[FLAG_STATIC].as<bool>());
    }

    if (checkNode(node, FLAG_SYNCHRONOUS))
    {
        newOperation->setSynchronous(node[FLAG_SYNCHRONOUS].as<bool>());
    }

    if (checkNode(node, KEY_PARAMS, YAML::NodeType::Sequence))
    {
        for (auto param : node[KEY_PARAMS])
        {
            newOperation->addParam(parseParameter(param));
        }
    }

    if (checkNode(node, KEY_RETURN, YAML::NodeType::Map))
    {
        newOperation->setResult(parseParameter(node[KEY_RETURN]));
    }

    return newOperation;
}


EventPtr Parser::parseEvent(const YAML::Node &node)
{
    cout << "parseEvent()" << endl;
    EventPtr newEvent = newIdentifiable<Event>(node);

    if (checkNode(node, FLAG_STATIC))
    {
        newEvent->setStatic(node[FLAG_STATIC].as<bool>());
    }

    if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
    {
        for (auto valueNode : node[KEY_VALUES])
        {
            newEvent->addResult(parseParameter(valueNode));
        }
    }

    return newEvent;
}


ValuePtr Parser::parseValue(const YAML::Node &node)
{
    cout << "parseValue()" << endl;
    ValuePtr newValue = newIdentifiable<Value>(node);

    if (node[KEY_VALUE].IsScalar())
    {
        newValue->setValue(node[KEY_VALUE].as<int32_t>());
        cout << "Value: " << newValue->longName() << " = " << newValue->value() << endl;
    }
    else
    {
        cout << "PARSE ERROR: value definition " << newValue->longName() << " has no value!" << endl;
        throw std::exception();
    }
    return newValue;
}


ParameterPtr Parser::parseParameter(const YAML::Node &node)
{
    cout << "parseParameter()" << endl;
    ParameterPtr newParameter = newIdentifiable<Parameter>(node);
    newParameter->setType(parseType(node));
    return newParameter;
}


TypePtr Parser::parseType(const YAML::Node &node)
{
    UnresolvedTypePtr newType = make_shared<UnresolvedType>();

    const YAML::Node typeNode = node[KEY_TYPE];

    switch (typeNode.Type())
    {
        case YAML::NodeType::Scalar:
            // this is a single type
            newType->setPrimary(typeNode.Scalar());
            break;

        case YAML::NodeType::Sequence:
            // this is a type with at least one subtype
            if (typeNode.size() >= 2)
            {
                for (int n = 0; n < typeNode.size(); ++n)
                {
                    if (typeNode[n].IsScalar())
                    {
                        if (n == 0)
                        {
                            newType->setPrimary(typeNode[n].Scalar());
                        }
                        else
                        {
                            newType->addParam(typeNode[n].Scalar());
                        }
                    }
                    else
                    {
                        cout << "ERROR: bad nested type definition" << endl;
                        throw std::exception();
                    }
                }
                cout << "New type " << newType->primary() << " with " << newType->params().size() << " subtypes" << endl;
            }
            break;

        default:
            cout << "ERROR: bad type definition" << endl;
            throw std::exception();
    }
    return newType;
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


bool Parser::checkNode(const YAML::Node &node, const char *key, YAML::NodeType::value expectedType, bool mandatory)
{
    if (node[key].Type() == expectedType)
    {
        return true;
    }

    if (!node[key].IsDefined())
    {
        if (mandatory)
        {
            cout << "ERROR: definition of " << key << " is missing!"<< endl;
            throw std::exception();
        }
    }
    else
    {
        cout << "ERROR: bad definition of " << key << endl;
        throw std::exception();
    }

    return false;
}


void Parser::registerType(NamespaceMemberPtr member)
{
    std::string type = getCurrentNamespace() + "::" + member->longName();

    if (mKnownTypes.find(type) == mKnownTypes.end())
    {
        cout << "REGISTERED TYPE: " << type << endl;
        mKnownTypes[type] = member;
    }
    else
    {
        cout << "ERROR: type " << type << " already exists!" << endl;
        throw std::exception();
    }
}


NamespaceMemberPtr Parser::resolveTypeName(string typeName)
{
    std::string key = getCurrentNamespace() + "::" + typeName;
    if (mKnownTypes.find(key) != mKnownTypes.end())
    {
        return mKnownTypes[key];
    }
    return nullptr;
}


void Parser::resolveParameterTypes(ParameterPtr parameter)
{
    UnresolvedTypePtr unresolvedType = dynamic_pointer_cast<UnresolvedType>(parameter->type());

    if (unresolvedType)
    {
        NamespaceMemberPtr member = resolveTypeName(unresolvedType->primary());

        if (member)
        {
            cout << "resolved primary type " << member->longName() <<  " of param " << parameter->longName() << endl;

            ResolvedTypePtr resolvedType = make_shared<ResolvedType>();
            resolvedType->setPrimary(member);

            // resolve list of typenames
            for (auto paramTypeName : unresolvedType->params())
            {
                member = resolveTypeName(paramTypeName);
                if (member)
                {
                    cout << "resolved subtype " << member->longName() <<  " of param " << parameter->longName() << endl;
                    resolvedType->addParam(member);
                }
                else
                {
                    cout << "ERROR: could not resolve subtype " << paramTypeName << " in parameter " << parameter->longName() << endl;
                    throw std::exception();
                }
            }

            parameter->setType(resolvedType);
        }
        else
        {
            cout << "ERROR: could not resolve primary type " << unresolvedType->primary() << " in parameter " << parameter->longName() << endl;
            throw std::exception();
        }
    }
}


void Parser::startNamespace(NamespacePtr namespaceRoot)
{
    mCurrentNamespaceElements.push_back(namespaceRoot->longName());
}


void Parser::endNamespace()
{
    mCurrentNamespaceElements.pop_back();
}


string Parser::getCurrentNamespace()
{
    string fullNamespace;
    for (auto namespaceElement : mCurrentNamespaceElements)
    {
        fullNamespace += "::" + namespaceElement;
    }
    return fullNamespace;
}


void Parser::resolveTypesInNamespace(NamespacePtr rootNamespace)
{
    NamespaceMemberPtr resolvedObject = nullptr;

    for (auto memberPair : rootNamespace->members())
    {
        // namespace
        NamespacePtr nestedNamespace = dynamic_pointer_cast<Namespace>(memberPair.second);
        if (nestedNamespace)
        {
            resolveTypesInNamespace(nestedNamespace);
        }
        else
        {
            // resolve events, operations & return
            cout << "resolving " << memberPair.first << endl;
            const ClassPtr &classPtr = dynamic_pointer_cast<Class>(memberPair.second);

            if (classPtr)
            {
                for (auto operation : classPtr->operations())
                {
                    for (auto parameter : operation.second->params())
                    {
                        resolveParameterTypes(parameter.second);
                    }

                    // resolve operation's return parameter
                    const ParameterPtr &resultParamPtr = operation.second->result();
                    if (resultParamPtr)
                    {
                        resolveParameterTypes(resultParamPtr);
                    }
                }

                for (auto event : classPtr->events())
                {
                    for (auto result : event.second->results())
                    {
                        resolveParameterTypes(result.second);
                    }
                }
            }
            else
            {
                // resolve structs
                const StructPtr &structPtr = dynamic_pointer_cast<Struct>(memberPair.second);
                if (structPtr)
                {
                    for (auto field : structPtr->fields())
                    {
                        resolveParameterTypes(field.second);
                    }
                }
            }
        }
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



