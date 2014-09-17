#include "parser/namespaceParser.hpp"

#define addFQNameToException(member, delimiter)     getCurrentNamespace() + "::" + member->longName() + delimiter + e.what()
#define addObjectNameToException(member)            member->longName() + " " + e.what()
#define addSectionToException(key)                  "in section " + string(key) + " : " + e.what()

using namespace std;
using namespace Api::Parser;
using namespace Api::Model;

const char *NamespaceParser::TYPE_NAMESPACE          = "namespace";
const char *NamespaceParser::TYPE_CLASS              = "class";
const char *NamespaceParser::TYPE_PRIMITIVE          = "primitive";
const char *NamespaceParser::TYPE_ENUM               = "enum";
const char *NamespaceParser::TYPE_STRUCT             = "struct";
const char *NamespaceParser::TYPE_CONTAINER          = "container";
const char *NamespaceParser::TYPE_CONSTANT           = "constant";

const char *NamespaceParser::KEY_NAME                = "name";
const char *NamespaceParser::KEY_SHORTNAME           = "short";
const char *NamespaceParser::KEY_NODETYPE            = "nodetype";
const char *NamespaceParser::KEY_CONTAINER_TYPE      = "containertype";
const char *NamespaceParser::KEY_PRIMITIVE_TYPE      = "primitivetype";
const char *NamespaceParser::KEY_TYPE                = "type";
const char *NamespaceParser::KEY_VALUE               = "value";
const char *NamespaceParser::KEY_RETURN              = "return";

const char *NamespaceParser::KEY_DOC                 = "doc";
const char *NamespaceParser::KEY_BRIEF               = "brief";
const char *NamespaceParser::KEY_MORE                = "more";

const char *NamespaceParser::KEY_MEMBERS             = "members";
const char *NamespaceParser::KEY_PARAMS              = "params";
const char *NamespaceParser::KEY_FIELDS              = "fields";
const char *NamespaceParser::KEY_VALUES              = "values";
const char *NamespaceParser::KEY_OPERATIONS          = "operations";
const char *NamespaceParser::KEY_EVENTS              = "events";

const char *NamespaceParser::FLAG_STATIC             = "static";
const char *NamespaceParser::FLAG_SYNCHRONOUS        = "synchronous";
const char *NamespaceParser::FLAG_VALUETYPE          = "valueType";
const char *NamespaceParser::KEY_INHERITS            = "inherits";


NamespaceParser::NamespaceParser(Model::NamespacePtr rootNamespace) :
    mRootNamespace(rootNamespace)
{
    // All first level entries have nodetypes TYPE_*
    // These nodetype strings are used to lookup the
    // corresponding parser function:
    mParserMethods[TYPE_NAMESPACE]  = &NamespaceParser::parseNamespace;
    mParserMethods[TYPE_CLASS]      = &NamespaceParser::parseClass;
    mParserMethods[TYPE_PRIMITIVE]  = &NamespaceParser::parsePrimitive;
    mParserMethods[TYPE_ENUM]       = &NamespaceParser::parseEnum;
    mParserMethods[TYPE_STRUCT]     = &NamespaceParser::parseStruct;
    mParserMethods[TYPE_CONTAINER]  = &NamespaceParser::parseContainer;
    mParserMethods[TYPE_CONSTANT]   = &NamespaceParser::parseConstant;
}


void NamespaceParser::parseFile(std::string filename)
{
    YAML::Node yamlConfig = loadFile(filename);

    try {
        parseNamespaceMembers(yamlConfig, mRootNamespace);
    }
    catch (const runtime_error &e)
    {
        throw;
    }
}


void NamespaceParser::setRootNamespace(Api::Model::NamespacePtr rootNamespace)
{
    mRootNamespace = rootNamespace;
}


void NamespaceParser::reset()
{
    mKnownTypes.clear();
    mNamespaceElementStack.clear();
}


void NamespaceParser::parseNamespaceMembers(const YAML::Node &node, NamespacePtr rootNamespace)
{
    for (auto sequenceNode : node)
    {
        if (checkNode(sequenceNode, KEY_NODETYPE, YAML::NodeType::Scalar, true))
        {
            string nodeType = sequenceNode[KEY_NODETYPE].as<string>();

            if (mParserMethods.find(nodeType) != mParserMethods.end())
            {
                MemberFunc func = mParserMethods[nodeType];
                NamespaceMemberPtr member = (this->*func)(sequenceNode);
                member->setParentIdentifiable(rootNamespace);
                rootNamespace->addMember(member);
            }
            else
            {
                cout << "WARNING: ignoring unknown nodetype " << nodeType << endl;
            }
        }
    }
}


NamespaceMemberPtr NamespaceParser::parseNamespace(const YAML::Node &node)
{
    string namespaceName;

    if (checkNode(node, KEY_NAME))
    {
        namespaceName = node[KEY_NAME].Scalar();
    }

    // check if we already know this namespace
    NamespacePtr newNamespace = dynamic_pointer_cast<Namespace>(resolveTypeName(namespaceName));

    if (!newNamespace)
    {
        // Namespace doesn't exist --> create new namespace object
        newNamespace = newIdentifiable<Namespace>(node);
        registerType(newNamespace);
    }

    startNamespace(newNamespace);

    if (checkNode(node, KEY_MEMBERS, YAML::NodeType::Sequence, true))
    {
        parseNamespaceMembers(node[KEY_MEMBERS], newNamespace);
    }

    endNamespace();
    return newNamespace;
}


NamespaceMemberPtr NamespaceParser::parseClass(const YAML::Node &node)
{
    ClassPtr newClass = newIdentifiable<Class>(node);
    registerType(newClass);

    try
    {
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
            newClass->setParent(newType);
        }

        if (checkNode(node, KEY_OPERATIONS, YAML::NodeType::Sequence))
        {
            for (auto operationNode : node[KEY_OPERATIONS])
            {
                OperationPtr newOperation = parseOperation(operationNode);
                newOperation->setParentIdentifiable(newClass);
                newClass->addOperation(newOperation);
            }
        }

        if (checkNode(node, KEY_EVENTS, YAML::NodeType::Sequence))
        {
            for (auto eventNode : node[KEY_EVENTS])
            {
                EventPtr newEvent = parseEvent(eventNode);
                newEvent->setParentIdentifiable(newClass);
                newClass->addEvent(newEvent);
            }
        }

        if (newClass->operations().size() == 0 &&
            newClass->events().size() == 0)
        {
            cout << "WARNING: no operations or events defined in class " << newClass->longName() << endl;
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newClass, "::"));
    }

    return newClass;
}


NamespaceMemberPtr NamespaceParser::parsePrimitive(const YAML::Node &node)
{
    // Primitives only consist of their names
    PrimitivePtr newPrimitive = newIdentifiable<Primitive>(node);
    registerType(newPrimitive);

    try {
        if (checkNode(node, KEY_PRIMITIVE_TYPE, YAML::NodeType::Scalar, true))
        {
            try {
                newPrimitive->setType(node[KEY_PRIMITIVE_TYPE].Scalar());
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(e.what() + Primitive::listSupportedTypes());
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newPrimitive, " : "));
    }

    return newPrimitive;
}


NamespaceMemberPtr NamespaceParser::parseEnum(const YAML::Node &node)
{
    EnumPtr newEnum = newIdentifiable<Enum>(node);
    registerType(newEnum);

    try
    {
        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto enumNode : node[KEY_VALUES])
                {
                    ValuePtr newValue = parseValue(enumNode);
                    newValue->setParentIdentifiable(newEnum);
                    newEnum->addValue(newValue);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addSectionToException(KEY_VALUES));
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newEnum, " : "));
    }

    return newEnum;
}


NamespaceMemberPtr NamespaceParser::parseStruct(const YAML::Node &node)
{
    StructPtr newStruct = newIdentifiable<Struct>(node);
    registerType(newStruct);

    try
    {
        if (checkNode(node, KEY_FIELDS, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto fieldNode : node[KEY_FIELDS])
                {
                    ParameterPtr newField = parseParameter(fieldNode);
                    newField->setParentIdentifiable(newStruct);
                    newStruct->addField(newField);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addSectionToException(KEY_FIELDS));
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newStruct, " : "));
    }

    return newStruct;
}


NamespaceMemberPtr NamespaceParser::parseContainer(const YAML::Node &node)
{
    ContainerPtr newContainer = newIdentifiable<Container>(node);
    registerType(newContainer);

    try
    {
        if (checkNode(node, KEY_CONTAINER_TYPE, YAML::NodeType::Scalar, true))
        {
            try {
                newContainer->setType(node[KEY_CONTAINER_TYPE].Scalar());
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(e.what() + Container::listSupportedTypes());
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newContainer, " : "));
    }

    return newContainer;
}


OperationPtr NamespaceParser::parseOperation(const YAML::Node &node)
{
    OperationPtr newOperation = newIdentifiable<Operation>(node);

    try
    {
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
            try {
                for (auto param : node[KEY_PARAMS])
                {
                    ParameterPtr newParam = parseParameter(param);
                    newParam->setParentIdentifiable(newOperation);
                    newOperation->addParam(newParam);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addSectionToException(KEY_PARAMS));
            }
        }

        if (checkNode(node, KEY_RETURN, YAML::NodeType::Map))
        {
            ParameterPtr newResult = parseParameter(node[KEY_RETURN]);
            newResult->setParentIdentifiable(newOperation);
            newOperation->setResult(newResult);
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addObjectNameToException(newOperation));
    }

    return newOperation;
}


EventPtr NamespaceParser::parseEvent(const YAML::Node &node)
{
    EventPtr newEvent = newIdentifiable<Event>(node);

    try
    {
        if (checkNode(node, FLAG_STATIC))
        {
            newEvent->setStatic(node[FLAG_STATIC].as<bool>());
        }

        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            for (auto valueNode : node[KEY_VALUES])
            {
                ParameterPtr newParam = parseParameter(valueNode);
                newParam->setParentIdentifiable(newEvent);
                newEvent->addResult(newParam);
            }
        }

        if (newEvent->results().size() == 0)
        {
            throw runtime_error("event " + newEvent->longName() + " has no result values\n");
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addObjectNameToException(newEvent));
    }

    return newEvent;
}


ValuePtr NamespaceParser::parseValue(const YAML::Node &node)
{
    ValuePtr newValue = newIdentifiable<Value>(node);

    if (node[KEY_VALUE].IsScalar())
    {
        newValue->setValue(node[KEY_VALUE].as<int32_t>());
    }
    else
    {
        throw runtime_error("value definition " + newValue->longName() + " has no value!\n");
    }
    return newValue;
}


ParameterPtr NamespaceParser::parseParameter(const YAML::Node &node)
{
    ParameterPtr newParameter = newIdentifiable<Parameter>(node);
    newParameter->setType(parseType(node));
    return newParameter;
}


TypePtr NamespaceParser::parseType(const YAML::Node &node)
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
                        throw runtime_error("bad nested type definition\n");
                    }
                }
            }
            break;

        default:
            throw runtime_error("bad type definition\n");
    }
    return newType;
}


NamespaceMemberPtr NamespaceParser::parseConstant(const YAML::Node &node)
{
    ConstantPtr newConstant = newIdentifiable<Constant>(node);
    registerType(newConstant);

    try
    {
        if (checkNode(node, KEY_TYPE, YAML::NodeType::Scalar, true))
        {
            UnresolvedTypePtr newType = make_shared<UnresolvedType>();
            newType->setPrimary(node[KEY_TYPE].Scalar());
            newConstant->setType(newType);
        }

        if (checkNode(node, KEY_VALUE, YAML::NodeType::Scalar, true))
        {
            newConstant->setValue(node[KEY_VALUE].Scalar());
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newConstant, " : "));
    }

    return newConstant;
}


void NamespaceParser::parseName(const YAML::Node &node, IdentifiablePtr identifiable)
{
    if (checkNode(node, KEY_NAME, YAML::NodeType::Scalar, true))
    {
        const string &name = node[KEY_NAME].Scalar();

        if (name.find("::") != string::npos)
        {
            throw runtime_error("please add a namespace section for each namespace part in " + name + "\n");
        }

        identifiable->setLongName(name);

        if (checkNode(node, KEY_SHORTNAME))
        {
            identifiable->setShortName(node[KEY_SHORTNAME].Scalar());
        }
    }
}


void NamespaceParser::parseDoc(const YAML::Node &node, IdentifiablePtr identifiable)
{
    if (checkNode(node, KEY_DOC, YAML::NodeType::Map))
    {
        const YAML::Node &docNode = node[KEY_DOC];
        DocumentationPtr newDoc = std::shared_ptr<Documentation>(new Documentation);

        if (checkNode(docNode, KEY_BRIEF))
        {
            newDoc->setBrief(docNode[KEY_BRIEF].Scalar());

            if (checkNode(docNode, KEY_MORE))
            {
                newDoc->setMore(docNode[KEY_MORE].Scalar());
            }
            identifiable->setDoc(newDoc);
        }
        else
        {
            throw runtime_error("documentation of " + identifiable->longName() + " is missing a brief description\n");
        }
    }
}


void NamespaceParser::registerType(NamespaceMemberPtr member)
{
    std::string type = getCurrentNamespace() + "::" + member->longName();

    if (mKnownTypes.find(type) == mKnownTypes.end())
    {
        mKnownTypes[type] = member;
    }
    else
    {
        throw runtime_error("type " + type + " already exists!\n");
    }
}


NamespaceMemberPtr NamespaceParser::resolveTypeName(string typeName)
{
    std::string key = getCurrentNamespace() + "::" + typeName;

    if (mKnownTypes.find(key) != mKnownTypes.end())
    {
        return mKnownTypes[key];
    }
    return nullptr;
}


ResolvedTypePtr NamespaceParser::resolveType(TypePtr type)
{
    UnresolvedTypePtr unresolvedType = dynamic_pointer_cast<UnresolvedType>(type);

    if (unresolvedType)
    {
        NamespaceMemberPtr member = resolveTypeName(unresolvedType->primary());

        if (member)
        {
//            cout << "resolved primary type " << member->longName() << endl;

            ResolvedTypePtr resolvedType = make_shared<ResolvedType>();
            resolvedType->setPrimary(member);

            // resolve list of typenames
            for (auto paramTypeName : unresolvedType->params())
            {
                member = resolveTypeName(paramTypeName);
                if (member)
                {
//                    cout << "resolved subtype " << member->longName() << endl;
                    resolvedType->addParam(member);
                }
                else
                {
                    throw runtime_error("could not resolve subtype " + paramTypeName + "\n");
                }
            }

            return resolvedType;
        }

        throw runtime_error("could not resolve primary type " + unresolvedType->primary() + "\n");
    }
    else
    {
        const ResolvedTypePtr &resolvedType = dynamic_pointer_cast<ResolvedType>(type);
        if (resolvedType)
        {
            return resolvedType;
        }
        else
        {
            throw runtime_error("resolveType(): parameter object has base type but must be derived type!\n");
        }
    }
}


void NamespaceParser::resolveParameterType(ParameterPtr parameter)
{
    try
    {
        parameter->setType(resolveType(parameter->type()));
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("in parameter " + parameter->longName() + ": " + e.what());
    }
}


void NamespaceParser::resolveTypesInNamespace(NamespacePtr rootNamespace)
{
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
            const ClassPtr &classPtr = dynamic_pointer_cast<Class>(memberPair.second);

            if (classPtr)
            {
                // resolve parent
                if (classPtr->parent())
                {
                    const ResolvedTypePtr &parentType = resolveType(classPtr->parent());

                    if (dynamic_pointer_cast<Class>(parentType->primary()))
                    {
                        classPtr->setParent(parentType);
                    }
                    else
                    {
                        throw runtime_error("inherited element in " + classPtr->longName() + " must point to a class\n");
                    }
                }

                for (auto operation : classPtr->operations())
                {
                    for (auto parameter : operation.second->params())
                    {
                        resolveParameterType(parameter.second);
                    }

                    // resolve operation's return parameter
                    const ParameterPtr &resultParamPtr = operation.second->result();
                    if (resultParamPtr)
                    {
                        resolveParameterType(resultParamPtr);
                    }
                }

                for (auto event : classPtr->events())
                {
                    for (auto result : event.second->results())
                    {
                        resolveParameterType(result.second);
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
                        resolveParameterType(field.second);
                    }
                }
            }
        }
    }
}


void NamespaceParser::listKnownTypes()
{
    cout << "------- REGISTERED TYPES -------" << endl;

    for (auto type : mKnownTypes)
    {
        cout << type.first << endl;
    }

    cout << "--------------------------------" << endl;
}


void NamespaceParser::startNamespace(NamespacePtr namespaceRoot)
{
    mNamespaceElementStack.push_back(namespaceRoot->longName());
}


void NamespaceParser::endNamespace()
{
    mNamespaceElementStack.pop_back();
}


string NamespaceParser::getCurrentNamespace()
{
    string fullNamespace;
    for (auto namespaceElement : mNamespaceElementStack)
    {
        fullNamespace += "::" + namespaceElement;
    }
    return fullNamespace;
}


template <class T>
std::shared_ptr<T> NamespaceParser::newIdentifiable(const YAML::Node &node)
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
        throw runtime_error("newIdentifiable(): Type is not an Identifiable!\n");
    }
    return newMember;
}



