#include "Components/NamespaceReader.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#define addFQNameToException(member, delimiter)     getCurrentNamespace() + "::" + member->longName() + delimiter + e.what()
#define addObjectNameToException(member)            member->longName() + " " + e.what()
#define addSectionToException(key)                  "in section " + string(key) + " : " + e.what()

using namespace std;
using namespace Everbase::InterfaceCompiler::Model;

namespace Everbase { namespace InterfaceCompiler { namespace Components {


const char *NamespaceReader::TYPE_NAMESPACE          = "namespace";
const char *NamespaceReader::TYPE_CLASS              = "class";
const char *NamespaceReader::TYPE_PRIMITIVE          = "primitive";
const char *NamespaceReader::TYPE_ENUM               = "enum";
const char *NamespaceReader::TYPE_STRUCT             = "struct";
const char *NamespaceReader::TYPE_CONTAINER          = "container";
const char *NamespaceReader::TYPE_CONSTANT           = "constant";

const char *NamespaceReader::KEY_NAME                = "name";
const char *NamespaceReader::KEY_SHORTNAME           = "short";
const char *NamespaceReader::KEY_NODETYPE            = "nodetype";
const char *NamespaceReader::KEY_CONTAINER_TYPE      = "containertype";
const char *NamespaceReader::KEY_PRIMITIVE_TYPE      = "primitivetype";
const char *NamespaceReader::KEY_TYPE                = "type";
const char *NamespaceReader::KEY_VALUE               = "value";
const char *NamespaceReader::KEY_RESULT              = "return";

const char *NamespaceReader::KEY_DOC                 = "doc";
const char *NamespaceReader::KEY_BRIEF               = "brief";
const char *NamespaceReader::KEY_MORE                = "more";

const char *NamespaceReader::KEY_MEMBERS             = "members";
const char *NamespaceReader::KEY_PARAMS              = "params";
const char *NamespaceReader::KEY_FIELDS              = "fields";
const char *NamespaceReader::KEY_VALUES              = "values";
const char *NamespaceReader::KEY_OPERATIONS          = "operations";
const char *NamespaceReader::KEY_EVENTS              = "events";

const char *NamespaceReader::FLAG_STATIC             = "static";
const char *NamespaceReader::FLAG_SYNCHRONOUS        = "synchronous";
const char *NamespaceReader::FLAG_VALUETYPE          = "valueType";
const char *NamespaceReader::KEY_INHERITS            = "inherits";

const char *NamespaceReader::KEY_ID                  = "id";


NamespaceReader::NamespaceReader(Model::NamespaceRef rootNamespace) :
    mRootNamespace(rootNamespace)
{
    // All first level entries have nodetypes TYPE_*
    // These nodetype strings are used to lookup the
    // corresponding parser function:
    mParserMethods[TYPE_NAMESPACE]  = &NamespaceReader::parseNamespace;
    mParserMethods[TYPE_CLASS]      = &NamespaceReader::parseClass;
    mParserMethods[TYPE_PRIMITIVE]  = &NamespaceReader::parsePrimitive;
    mParserMethods[TYPE_ENUM]       = &NamespaceReader::parseEnum;
    mParserMethods[TYPE_STRUCT]     = &NamespaceReader::parseStruct;
    mParserMethods[TYPE_CONTAINER]  = &NamespaceReader::parseContainer;
    mParserMethods[TYPE_CONSTANT]   = &NamespaceReader::parseConstant;
}


void NamespaceReader::parseFile(std::istream& stream)
{
    YAML::Node yamlConfig = loadFile(stream);

    try {
        parseNamespaceMembers(yamlConfig, mRootNamespace);
    }
    catch (const runtime_error &e)
    {
        throw;
    }
}


void NamespaceReader::setRootNamespace(Model::NamespaceRef rootNamespace)
{
    mRootNamespace = rootNamespace;
}


void NamespaceReader::reset()
{
    mKnownTypes.clear();
    mNamespaceElementStack.clear();
}


void NamespaceReader::parseNamespaceMembers(const YAML::Node &node, NamespaceRef rootNamespace)
{
    for (auto sequenceNode : node)
    {
        if (checkNode(sequenceNode, KEY_NODETYPE, YAML::NodeType::Scalar, true))
        {
            string nodeType = sequenceNode[KEY_NODETYPE].as<string>();

            if (mParserMethods.find(nodeType) != mParserMethods.end())
            {
                MemberFunc func = mParserMethods[nodeType];
                NamespaceMemberRef member = (this->*func)(sequenceNode);
                member->setParentObject(rootNamespace);
                rootNamespace->addMember(member);
            }
            else
            {
                cout << "WARNING: ignoring unknown nodetype " << nodeType << endl;
            }
        }
    }
}


NamespaceMemberRef NamespaceReader::parseNamespace(const YAML::Node &node)
{
    string namespaceName;

    if (checkNode(node, KEY_NAME))
    {
        namespaceName = node[KEY_NAME].Scalar();
    }

    // check if we already know this namespace
    NamespaceRef newNamespace = dynamic_pointer_cast<Namespace>(resolveTypeName(namespaceName));

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


NamespaceMemberRef NamespaceReader::parseClass(const YAML::Node &node)
{
    ClassRef newClass = newIdentifiable<Class>(node);
    registerType(newClass);

    try
    {
        if (node[FLAG_VALUETYPE].IsScalar() && node[FLAG_VALUETYPE].as<bool>())
        {
            newClass->setType(Class::ClassType::VALUE);
        }
        else
        {
            newClass->setType(Class::ClassType::INTERFACE);
        }

        if (checkNode(node, KEY_INHERITS))
        {
            UnresolvedTypeRef newType = make_shared<UnresolvedType>();
            newType->setPrimary(node[KEY_INHERITS].Scalar());
            newClass->setParent(newType);
        }

        if (checkNode(node, KEY_OPERATIONS, YAML::NodeType::Sequence))
        {
            for (auto operationNode : node[KEY_OPERATIONS])
            {
                OperationRef newOperation = parseOperation(operationNode);
                newOperation->setParentObject(newClass);
                newClass->addOperation(newOperation);
            }
        }

        if (checkNode(node, KEY_EVENTS, YAML::NodeType::Sequence))
        {
            for (auto eventNode : node[KEY_EVENTS])
            {
                EventRef newEvent = parseEvent(eventNode);
                newEvent->setParentObject(newClass);
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


NamespaceMemberRef NamespaceReader::parsePrimitive(const YAML::Node &node)
{
    // Primitives only consist of their names
    PrimitiveRef newPrimitive = newIdentifiable<Primitive>(node);
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


NamespaceMemberRef NamespaceReader::parseEnum(const YAML::Node &node)
{
    EnumRef newEnum = newIdentifiable<Enum>(node);
    registerType(newEnum);

    try
    {
        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto enumNode : node[KEY_VALUES])
                {
                    ValueRef newValue = parseValue(enumNode);
                    newValue->setParentObject(newEnum);
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


NamespaceMemberRef NamespaceReader::parseStruct(const YAML::Node &node)
{
    StructRef newStruct = newIdentifiable<Struct>(node);
    registerType(newStruct);

    try
    {
        if (checkNode(node, KEY_FIELDS, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto fieldNode : node[KEY_FIELDS])
                {
                    ParameterRef newField = parseParameter(fieldNode);
                    newField->setParentObject(newStruct);
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


NamespaceMemberRef NamespaceReader::parseContainer(const YAML::Node &node)
{
    ContainerRef newContainer = newIdentifiable<Container>(node);
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


OperationRef NamespaceReader::parseOperation(const YAML::Node &node)
{
    OperationRef newOperation = newIdentifiable<Operation>(node);

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
                    ParameterRef newParam = parseParameter(param);
                    newParam->setParentObject(newOperation);
                    newOperation->addParam(newParam);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addSectionToException(KEY_PARAMS));
            }
        }

        if (checkNode(node, KEY_RESULT, YAML::NodeType::Map))
        {
            ParameterRef newResult = parseParameter(node[KEY_RESULT]);
            newResult->setParentObject(newOperation);
            newOperation->setResult(newResult);
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addObjectNameToException(newOperation));
    }

    return newOperation;
}


EventRef NamespaceReader::parseEvent(const YAML::Node &node)
{
    EventRef newEvent = newIdentifiable<Event>(node);

    try
    {
        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            for (auto valueNode : node[KEY_VALUES])
            {
                ParameterRef newParam = parseParameter(valueNode);
                newParam->setParentObject(newEvent);
                newEvent->addValue(newParam);
            }
        }

        if (checkNode(node, KEY_ID, YAML::NodeType::Scalar, true))
        {
            newEvent->setTypeId(boost::lexical_cast<boost::uuids::uuid>(node[KEY_ID].Scalar()));
        }

        if (newEvent->values().size() == 0)
        {
            throw runtime_error("event " + newEvent->longName() + " has no values\n");
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addObjectNameToException(newEvent));
    }

    return newEvent;
}


ValueRef NamespaceReader::parseValue(const YAML::Node &node)
{
    ValueRef newValue = newIdentifiable<Value>(node);

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


ParameterRef NamespaceReader::parseParameter(const YAML::Node &node)
{
    ParameterRef newParameter = newIdentifiable<Parameter>(node);
    newParameter->setType(parseType(node));
    return newParameter;
}


TypeRef NamespaceReader::parseType(const YAML::Node &node)
{
    UnresolvedTypeRef newType = make_shared<UnresolvedType>();

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
                for (size_t n = 0; n < typeNode.size(); ++n)
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


NamespaceMemberRef NamespaceReader::parseConstant(const YAML::Node &node)
{
    ConstantRef newConstant = newIdentifiable<Constant>(node);
    registerType(newConstant);

    try
    {
        if (checkNode(node, KEY_TYPE, YAML::NodeType::Scalar, true))
        {
            UnresolvedTypeRef newType = make_shared<UnresolvedType>();
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


void NamespaceReader::parseName(const YAML::Node &node, IdentifiableRef identifiable)
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


void NamespaceReader::parseDoc(const YAML::Node &node, IdentifiableRef identifiable)
{
    if (checkNode(node, KEY_DOC, YAML::NodeType::Map))
    {
        DocumentationRef newDoc = std::shared_ptr<Documentation>(new Documentation);

        for (auto mapEntry : node[KEY_DOC])
        {
            newDoc->addDocEntry(mapEntry.first.Scalar(), mapEntry.second.Scalar());
        }

        if (newDoc->docEntries().size())
        {
            identifiable->setDoc(newDoc);
        }
    }
}


void NamespaceReader::registerType(NamespaceMemberRef member)
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


NamespaceMemberRef NamespaceReader::resolveTypeName(string typeName)
{
    std::string key = getCurrentNamespace() + "::" + typeName;

    if (mKnownTypes.find(key) != mKnownTypes.end())
    {
        return mKnownTypes[key];
    }
    return nullptr;
}


TypeRef NamespaceReader::resolveType(TypeBaseRef type)
{
    UnresolvedTypeRef unresolvedType = dynamic_pointer_cast<UnresolvedType>(type);

    if (unresolvedType)
    {
        NamespaceMemberRef member = resolveTypeName(unresolvedType->primary());

        if (member)
        {
//            cout << "resolved primary type " << member->longName() << endl;

            TypeRef resolvedType = make_shared<Type>();
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
        else
        {
            throw runtime_error("could not resolve primary type " + unresolvedType->primary() + "\n");
        }
    }
    else
    {
        const TypeRef &resolvedType = dynamic_pointer_cast<Type>(type);
        if (resolvedType)
        {
            cout << "resolved to existing type" << endl;
            return resolvedType;
        }
        else
        {
            throw runtime_error("resolveType(): parameter object has base type but must be derived type!\n");
        }
    }
}


void NamespaceReader::resolveParameterType(ParameterRef parameter)
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


void NamespaceReader::resolveTypesInNamespace(NamespaceRef rootNamespace)
{
    for (auto namespaceMember : rootNamespace->members())
    {
        // namespace
        NamespaceRef nestedNamespace = dynamic_pointer_cast<Namespace>(namespaceMember);
        if (nestedNamespace)
        {
            try {
                resolveTypesInNamespace(nestedNamespace);
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addFQNameToException(nestedNamespace, ""));
            }
        }
        else if (const ClassRef &classRef = dynamic_pointer_cast<Class>(namespaceMember))
        {
            // resolve events, operations & return
            try {
                // resolve parent
                if (classRef->parent())
                {
                    const TypeRef &parentType = resolveType(classRef->parent());

                    if (dynamic_pointer_cast<Class>(parentType->primary()))
                    {
                        classRef->setParent(parentType);
                    }
                    else
                    {
                        throw runtime_error("inherited element in " + classRef->longName() + " must point to a class\n");
                    }
                }

                for (auto operation : classRef->operations())
                {
                    try {
                        for (auto parameter : operation->params())
                        {
                            resolveParameterType(parameter);
                        }

                        // resolve operation's return parameter
                        const ParameterRef &resultParamRef = operation->result();

                        if (resultParamRef)
                        {
                            resolveParameterType(resultParamRef);
                        }
                    }
                    catch (const runtime_error &e)
                    {
                        throw runtime_error(addObjectNameToException(operation));
                    }
                }

                for (auto event : classRef->events())
                {
                    for (auto value : event->values())
                    {
                        resolveParameterType(value);
                    }
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addFQNameToException(classRef, "::"));
            }
        }
        else if (const StructRef &structRef = dynamic_pointer_cast<Struct>(namespaceMember))
        {
            // resolve structs
            try {
                for (auto field : structRef->fields())
                {
                    resolveParameterType(field);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addFQNameToException(namespaceMember, " "));
            }
        }
        else if (const ConstantRef &constantRef = dynamic_pointer_cast<Constant>(namespaceMember))
        {
            // resolve constants
            try {
                const TypeRef &resolvedType = resolveType(constantRef->type());

                if (resolvedType)
                {
                    constantRef->setType(resolvedType);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addFQNameToException(namespaceMember, " "));
            }
        }
    }
}


void NamespaceReader::listKnownTypes()
{
    cout << "------- REGISTERED TYPES -------" << endl;

    for (auto type : mKnownTypes)
    {
        cout << type.first << endl;
    }

    cout << "--------------------------------" << endl;
}


void NamespaceReader::startNamespace(NamespaceRef namespaceRoot)
{
    mNamespaceElementStack.push_back(namespaceRoot->longName());
}


void NamespaceReader::endNamespace()
{
    mNamespaceElementStack.pop_back();
}


string NamespaceReader::getCurrentNamespace()
{
    string fullNamespace;
    for (auto namespaceElement : mNamespaceElementStack)
    {
        fullNamespace += "::" + namespaceElement;
    }
    return fullNamespace;
}

} } } // namespace: Everbase::InterfaceCompiler::Components

