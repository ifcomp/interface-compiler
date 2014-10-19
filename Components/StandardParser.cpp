#include "Components/StandardParser.hpp"

#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>

#define addFQNameToException(member)    getQualifiedName(member) + " : " + e.what()
#define addSectionToException(key)      "in section " + string(key) + " : " + e.what()

using namespace std;
using namespace Everbase::InterfaceCompiler::Model;

namespace Everbase { namespace InterfaceCompiler { namespace Components {


const char *StandardParser::TYPE_NAMESPACE          = "namespace";
const char *StandardParser::TYPE_PRIMITIVE          = "primitive";
const char *StandardParser::TYPE_ENUM               = "enum";
const char *StandardParser::TYPE_STRUCT             = "struct";
const char *StandardParser::TYPE_CLASS              = "class";

const char *StandardParser::KEY_NAME                = "name";
const char *StandardParser::KEY_SHORTNAME           = "short";
const char *StandardParser::KEY_UNDERLYING          = "underlying";
const char *StandardParser::KEY_TYPE                = "type";
const char *StandardParser::KEY_VALUE               = "value";
const char *StandardParser::KEY_RESULT              = "return";

const char *StandardParser::KEY_DOC                 = "doc";
const char *StandardParser::KEY_BRIEF               = "brief";
const char *StandardParser::KEY_MORE                = "more";

const char *StandardParser::KEY_MEMBERS             = "members";
const char *StandardParser::KEY_PARAMS              = "params";
const char *StandardParser::KEY_FIELDS              = "fields";
const char *StandardParser::KEY_VALUES              = "values";
const char *StandardParser::KEY_OPERATIONS          = "operations";
const char *StandardParser::KEY_EVENTS              = "events";
const char *StandardParser::KEY_CONSTANTS           = "constants";

const char *StandardParser::FLAG_STATIC             = "static";
const char *StandardParser::FLAG_SYNCHRONOUS        = "synchronous";
const char *StandardParser::FLAG_VALUETYPE          = "valueType";
const char *StandardParser::KEY_SUPER               = "super";

const char *StandardParser::KEY_ID                  = "id";


Model::RootRef StandardParser::execute ( std::istream& input ) const
{
    RootRef root = parseFile(input);
    resolve(root);
    return root;
}


RootRef StandardParser::parseFile(istream &stream) const
{
    try {
        try {
            YAML::Node yamlConfig = YAML::Load(stream);

            auto root = make_shared<Root>();
            parseElements(yamlConfig, nullptr, root);
            return root;
        }
        catch (const runtime_error &e)
        {
            throw;
        }
    }
    catch (const YAML::Exception &e)
    {
        throw runtime_error(std::string("Something's wrong with your YAML input file: ") + e.what());
    }
}


void StandardParser::resolve(const RootRef &root)
{
    try {
        resolveTypesInNamespace(root->getNamespace(), root);
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(std::string("Type resolver: ") + e.what());
    }
}


bool StandardParser::checkNode(const YAML::Node &node, const char *key, YAML::NodeType::value expectedType, bool mandatory)
{
    if (node[key].Type() == expectedType)
    {
        return true;
    }

    if (!node[key].IsDefined())
    {
        if (mandatory)
        {
            throw runtime_error("definition of key '" + string(key) + "' is missing!\n");
        }
    }
    else
    {
        throw runtime_error("bad definition of key '" + string(key) + "'\n");
    }

    return false;
}


string StandardParser::getQualifiedName(const IdentifiableRef &identifiable, const IdentifiableRef &parent)
{
    string output = getNamespace(parent ? parent : identifiable);
    if (parent)
    {
        output += parent->longName() + "::";
    }
    return output + identifiable->longName();
}


string StandardParser::getNamespace(const IdentifiableRef &identifiable)
{
    string output;

    if (identifiable)
    {
        ObjectRef parent = identifiable->parent();

        while (auto parentIdentifiable = dynamic_pointer_cast<Identifiable>(parent))
        {
            output = parentIdentifiable->longName() + "::" + output;
            parent = parent->parent();
        }
    }
    return output;
}


void StandardParser::parseElements(const YAML::Node &node, const NamespaceRef &parentNamespace, const RootRef &root) const
{
    std::map<std::string, MemberFunc> parserMethods;
    parserMethods[TYPE_NAMESPACE]  = &StandardParser::parseNamespace;
    parserMethods[TYPE_CLASS]      = &StandardParser::parseClass;
    parserMethods[TYPE_PRIMITIVE]  = &StandardParser::parsePrimitive;
    parserMethods[TYPE_ENUM]       = &StandardParser::parseEnum;
    parserMethods[TYPE_STRUCT]     = &StandardParser::parseStruct;

    for (auto sequenceNode : node)
    {
        if (checkNode(sequenceNode, KEY_TYPE, YAML::NodeType::Scalar, true))
        {
            string nodeType = sequenceNode[KEY_TYPE].as<string>();

            if (parserMethods.find(nodeType) != parserMethods.end())
            {
                MemberFunc func = parserMethods[nodeType];
                (this->*func)(sequenceNode, parentNamespace, root);
            }
            else
            {
                throw runtime_error("unknown type " + nodeType + "\n");
            }
        }
    }
}


void StandardParser::parseNamespace(const YAML::Node &node, const ElementRef &parent, const RootRef &root) const
{
    NamespaceRef newNamespace = newIdentifiable<Namespace>(node);
    NamespaceRef existingNamespace = dynamic_pointer_cast<Namespace>(resolveTypeName(getQualifiedName(newNamespace, parent), root));

    if (existingNamespace)
    {
        newNamespace = existingNamespace;
    }

    if (checkNode(node, KEY_MEMBERS, YAML::NodeType::Sequence, true))
    {
        if (parent)
        {
            if (NamespaceRef parentNamespace = dynamic_pointer_cast<Namespace>(parent))
            {

                if (existingNamespace)
                {
                    newNamespace = existingNamespace;
                }
                else
                {
                    // only add the new namespace element if this namespace does not yet exist
                    parentNamespace->addElement(newNamespace);
                }
            }
            else
            {
                throw runtime_error("parseNamespace() : unsupported parent element");
            }
        }
        else
        {
            if (!root->getNamespace())
            {
                // set root namespace
                root->setNamespace(newNamespace);
            }
            else
            {
                if (newNamespace != root->getNamespace())
                {
                    throw runtime_error("found root namespace " + newNamespace->longName() +
                                        " but root node already contains namespace " +
                                        root->getNamespace()->longName() + "\n");
                }
            }
        }

        parseElements(node[KEY_MEMBERS], newNamespace, root);
    }
}


void StandardParser::parseClass(const YAML::Node &node, const ElementRef &parent, const RootRef &root) const
{
    ClassRef newClass = newIdentifiable<Class>(node);

    if (NamespaceRef parentNamespace = dynamic_pointer_cast<Namespace>(parent))
    {
        // add class to tree before adding children
        // otherwise class' namespace won't get resolved in error messages
        parentNamespace->addElement(newClass);
    }
    else
    {
        throw runtime_error("parseClass() : unsupported parent element");
    }

//        if (node[FLAG_VALUETYPE].IsScalar() && node[FLAG_VALUETYPE].as<bool>())
//        {
//            newClass->setBehavior(Class::Behavior::VALUE);
//        }
//        else
//        {
//            newClass->setBehavior(Class::Behavior::INTERFACE);
//        }

    if (checkNode(node, KEY_SUPER))
    {
        UnresolvedTypeRef newType = make_shared<UnresolvedType>();
        newType->setPrimary(node[KEY_SUPER].Scalar());
        newClass->setSuper(newType);
    }

    if (checkNode(node, KEY_OPERATIONS, YAML::NodeType::Sequence))
    {
        for (auto operationNode : node[KEY_OPERATIONS])
        {
            parseClassOperation(operationNode, newClass);
        }
    }

    if (checkNode(node, KEY_EVENTS, YAML::NodeType::Sequence))
    {
        for (auto eventNode : node[KEY_EVENTS])
        {
            parseClassEvent(eventNode, newClass);
        }
    }

    if (checkNode(node, KEY_CONSTANTS, YAML::NodeType::Sequence))
    {
        for (auto constantNode : node[KEY_CONSTANTS])
        {
            parseClassConstant(constantNode, newClass);
        }
    }
}


void StandardParser::parsePrimitive(const YAML::Node &node, const ElementRef &parent, const RootRef &root) const
{
    // Primitives only consist of their names
    PrimitiveRef newPrimitive = newIdentifiable<Primitive>(node);

    if (NamespaceRef parentNamespace = dynamic_pointer_cast<Namespace>(parent))
    {
        parentNamespace->addElement(newPrimitive);
    }
    else
    {
        throw runtime_error("parsePrimitive() : unsupported parent element");
    }

    try {
        if (checkNode(node, KEY_UNDERLYING, YAML::NodeType::Scalar, true))
        {
            try {
                newPrimitive->setUnderlying(node[KEY_UNDERLYING].Scalar());
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(e.what() + Primitive::listSupportedUnderlying());
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newPrimitive));
    }
}


void StandardParser::parseEnum(const YAML::Node &node, const ElementRef &parent, const RootRef &root) const
{
    EnumRef newEnum = newIdentifiable<Enum>(node);

    if (NamespaceRef parentNamespace = dynamic_pointer_cast<Namespace>(parent))
    {
        parentNamespace->addElement(newEnum);
    }
    else
    {
        throw runtime_error("parseEnum() : unsupported parent element");
    }

    try
    {
        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto enumNode : node[KEY_VALUES])
                {
                    parseEnumValue(enumNode, newEnum);
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
        throw runtime_error(addFQNameToException(newEnum));
    }
}


void StandardParser::parseStruct(const YAML::Node &node, const ElementRef &parent, const RootRef &root) const
{
    StructRef newStruct = newIdentifiable<Struct>(node);

    if (NamespaceRef parentNamespace = dynamic_pointer_cast<Namespace>(parent))
    {
        parentNamespace->addElement(newStruct);
    }
    else
    {
        throw runtime_error("parseStruct() : unsupported parent element");
    }

    try
    {
        if (checkNode(node, KEY_FIELDS, YAML::NodeType::Sequence, true))
        {
            try
            {
                for (auto fieldNode : node[KEY_FIELDS])
                {
                    ParameterRef newField = parseParameter(fieldNode);
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
        throw runtime_error(addFQNameToException(newStruct));
    }
}


void StandardParser::parseClassOperation(const YAML::Node &node, const ClassRef &parent) const
{
    Class::OperationRef newOperation = newIdentifiable<Class::Operation>(node);
    parent->addOperation(newOperation);

    try
    {
        if (checkNode(node, FLAG_STATIC))
        {
            newOperation->setStatic(node[FLAG_STATIC].as<bool>());
        }

//        if (checkNode(node, FLAG_SYNCHRONOUS))
//        {
//            newOperation->setSynchronous(node[FLAG_SYNCHRONOUS].as<bool>());
//        }

        if (checkNode(node, KEY_PARAMS, YAML::NodeType::Sequence))
        {
            try {
                for (auto param : node[KEY_PARAMS])
                {
                    ParameterRef newParam = parseParameter(param);
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
            newOperation->setResult(newResult);
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newOperation));
    }
}


void StandardParser::parseClassEvent(const YAML::Node &node, const ClassRef &parent) const
{
    Class::EventRef newEvent = newIdentifiable<Class::Event>(node);
    parent->addEvent(newEvent);

    try
    {
        if (checkNode(node, KEY_VALUES, YAML::NodeType::Sequence, true))
        {
            for (auto valueNode : node[KEY_VALUES])
            {
                ParameterRef newParam = parseParameter(valueNode);
                newEvent->addValue(newParam);
            }
        }

        if (checkNode(node, KEY_ID, YAML::NodeType::Scalar, true))
        {
            newEvent->setTypeId(boost::lexical_cast<boost::uuids::uuid>(node[KEY_ID].Scalar()));
        }

        if (newEvent->values().size() == 0)
        {
            throw runtime_error("event has no values\n");
        }
    }
    catch (const exception &e)
    {
        throw runtime_error(addFQNameToException(newEvent));
    }
}


void StandardParser::parseClassConstant(const YAML::Node &node, const ClassRef &parent) const
{
    Class::ConstantRef newConstant = newIdentifiable<Class::Constant>(node);
    parent->addConstant(newConstant);

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
            string value = node[KEY_VALUE].Scalar();
            newConstant->setValue(value);
            // value will get translated in resolve procedure
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error(addFQNameToException(newConstant));
    }
}


void StandardParser::parseEnumValue(const YAML::Node &node, const EnumRef &parent) const
{
    Enum::ValueRef newValue = newIdentifiable<Enum::Value>(node);
    parent->addValue(newValue);

    if (node[KEY_VALUE].IsScalar())
    {
        newValue->setValue(node[KEY_VALUE].as<int32_t>());
    }
    else
    {
        throw runtime_error("value definition " + newValue->longName() + " has no value!\n");
    }
}


ParameterRef StandardParser::parseParameter(const YAML::Node &node) const
{
    ParameterRef newParameter = newIdentifiable<Parameter>(node);
    newParameter->setType(parseType(node));

    return newParameter;
}


TypeBaseRef StandardParser::parseType(const YAML::Node &node) const
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


void StandardParser::parseName(const YAML::Node &node, const IdentifiableRef &identifiable)
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


void StandardParser::parseDoc(const YAML::Node &node, const IdentifiableRef &identifiable)
{
    if (checkNode(node, KEY_DOC, YAML::NodeType::Map))
    {
        DocumentationRef newDoc = std::shared_ptr<Documentation>(new Documentation);

        for (auto mapEntry : node[KEY_DOC])
        {
            if (mapEntry.first.Scalar() == Documentation::KEY_PARAM)
            {
                throw runtime_error("key " + string(Documentation::KEY_PARAM) + " is not allowed here! (gets generated from parameter object doc)\n");
            }

            newDoc->addDocEntry(mapEntry.first.Scalar(), mapEntry.second.Scalar());
        }

        if (newDoc->docEntries().size())
        {
            identifiable->setDoc(newDoc);
        }
    }
}


ElementRef StandardParser::findElement(const ElementRef &parent, string name, bool matchSiblings)
{
    if (parent)
    {
        if (!matchSiblings)
        {
            if (parent->longName() == name)
            {
                return parent;
            }
        }
        else
        {
            if (const NamespaceRef &nestedNamespace = dynamic_pointer_cast<Namespace>(parent))
            {
                for (auto element : nestedNamespace->elements())
                {
                    if (element->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(element);
                    }
                }
            }
            else if (ClassRef classElement = dynamic_pointer_cast<Class>(parent))
            {
                for (auto operation : classElement->operations())
                {
                    if (operation->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(operation);
                    }
                }

                for (auto event : classElement->events())
                {
                    if (event->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(event);
                    }
                }

                for (auto constant : classElement->constants())
                {
                    if (constant->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(constant);
                    }
                }
            }
            else if (StructRef structElement = dynamic_pointer_cast<Struct>(parent))
            {
                for (auto field : structElement->fields())
                {
                    if (field->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(field);
                    }
                }
            }
            else if (EnumRef enumElement = dynamic_pointer_cast<Enum>(parent))
            {
                for (auto value : enumElement->values())
                {
                    if (value->longName() == name)
                    {
                        return dynamic_pointer_cast<Element>(value);
                    }
                }
            }
            else if (PrimitiveRef primitiveElement = dynamic_pointer_cast<Primitive>(parent))
            {
                if (primitiveElement->longName() == name)
                {
                    return primitiveElement;
                }
            }
            else
            {
                throw runtime_error("findElement() : unknown element type " + parent->typeName() + "\n");
            }
        }
    }
    return nullptr;
}


ElementRef StandardParser::resolveTypeName(string typeName, const RootRef &root)
{
    vector<string> nameParts;
    boost::replace_all(typeName, "::", ":");
    boost::split(nameParts, typeName, boost::is_any_of(":"));

    ElementRef element = root->getNamespace();

    for (std::uint16_t n = 0; n < nameParts.size(); ++n)
    {
        element = findElement(element, nameParts[n], n);
    }

    return element;
}


TypeRef StandardParser::resolveType(const TypeBaseRef &type, const RootRef &root)
{
    UnresolvedTypeRef unresolvedType = dynamic_pointer_cast<UnresolvedType>(type);

    if (unresolvedType)
    {
        ElementRef member = resolveTypeName(unresolvedType->primary(), root);

        if (member)
        {
            TypeRef resolvedType = make_shared<Type>();
            resolvedType->setPrimary(member);

            // resolve list of typenames
            for (auto paramTypeName : unresolvedType->params())
            {
                member = resolveTypeName(paramTypeName, root);
                if (member)
                {
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
        TypeRef resolvedType = dynamic_pointer_cast<Type>(type);
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


void StandardParser::resolveParameterType(const ParameterRef &parameter, const RootRef &root)
{
    try
    {
        parameter->setType(resolveType(parameter->type(), root));
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("in parameter " + parameter->longName() + ": " + e.what());
    }
}


void StandardParser::resolveTypesInNamespace(const NamespaceRef &rootNamespace, const RootRef &root)
{
    for (auto element : rootNamespace->elements())
    {
        // namespace
        if (auto nestedNamespace = dynamic_pointer_cast<Namespace>(element))
        {
            resolveTypesInNamespace(nestedNamespace, root);
        }
        else if (const ClassRef &classRef = dynamic_pointer_cast<Class>(element))
        {
            // resolve super
            if (classRef->super())
            {
                TypeRef superType = resolveType(classRef->super(), root);

                if (dynamic_pointer_cast<Class>(superType->primary()))
                {
                    classRef->setSuper(superType);
                }
                else
                {
                    throw runtime_error("inherited element in " + classRef->longName() + " must point to a class\n");
                }
            }

            // resolve class operations
            for (auto operation : classRef->operations())
            {
                try {
                    for (auto parameter : operation->params())
                    {
                        resolveParameterType(parameter, root);
                    }

                    // resolve operation's return parameter
                    const ParameterRef &resultParamRef = operation->result();

                    if (resultParamRef)
                    {
                        resolveParameterType(resultParamRef, root);
                    }
                }
                catch (const runtime_error &e)
                {
                    throw runtime_error(addFQNameToException(operation));
                }
            }

            // resolve class events
            for (auto event : classRef->events())
            {

                try {
                    for (auto value : event->values())
                    {
                        resolveParameterType(value, root);
                    }
                }
                catch (const runtime_error &e)
                {
                    throw runtime_error(addFQNameToException(event));
                }
            }

            // resolve class constants
            for (auto constant : classRef->constants())
            {
                try {
                    TypeRef resolvedType = resolveType(constant->type(), root);

                    if (resolvedType)
                    {
                        constant->setType(resolvedType);

                        // convert value
                        if (constant->value().type() == typeid(std::string))
                        {
                            std::string value = boost::any_cast<std::string>(constant->value());

                            if( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
                            {
                                switch( primitive->underlying() )
                                {
                                    case Primitive::Underlying::BYTE:
                                        constant->setValue(boost::lexical_cast<std::uint8_t>(value));
                                        break;

                                    case Primitive::Underlying::UINT16:
                                        constant->setValue(boost::lexical_cast<std::uint16_t>(value));
                                        break;

                                    case Primitive::Underlying::UINT32:
                                        constant->setValue(boost::lexical_cast<std::uint32_t>(value));
                                        break;

                                    case Primitive::Underlying::UINT64:
                                        constant->setValue(boost::lexical_cast<std::uint64_t>(value));
                                        break;

                                    case Primitive::Underlying::BOOLEAN:
                                        constant->setValue(boost::lexical_cast<bool>(value));
                                        break;

                                    case Primitive::Underlying::TIMESTAMP:
                                        throw std::runtime_error("not supported");

                                    case Primitive::Underlying::STRING:
                                        constant->setValue(value);
                                        break;

                                    case Primitive::Underlying::TYPEID:
                                        constant->setValue(boost::lexical_cast<boost::uuids::uuid>(value));
                                        break;

                                    default:
                                        throw std::runtime_error("not supported");
                                }
                            }
                        }
                    }
                }
                catch (const exception &e)
                {
                    throw runtime_error(addFQNameToException(constant));
                }
            }
        }
        else if (const StructRef &structRef = dynamic_pointer_cast<Struct>(element))
        {
            // resolve structs
            try {
                for (auto field : structRef->fields())
                {
                    resolveParameterType(field, root);
                }
            }
            catch (const runtime_error &e)
            {
                throw runtime_error(addFQNameToException(structRef));
            }
        }
    }
}


} } } // namespace: Everbase::InterfaceCompiler::Components
