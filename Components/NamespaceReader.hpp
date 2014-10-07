#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <functional>
#include <map>

#include "Model/Namespace.hpp"
#include "Model/Class.hpp"
#include "Model/Class/Operation.hpp"
#include "Model/Class/Event.hpp"
#include "Model/Class/Constant.hpp"
#include "Model/Enum.hpp"
#include "Model/Enum/Value.hpp"
#include "Model/Struct.hpp"
#include "Model/Primitive.hpp"
#include "Model/Documentation.hpp"
#include "Model/Parameter.hpp"
#include "Model/Type.hpp"
#include "Model/UnresolvedType.hpp"
#include "Model/Type.hpp"


namespace Everbase { namespace InterfaceCompiler { namespace Components {

/**
 * @brief YAML parser class for namespace objects
 * @author Gunther Lemm <lemm@silpion.de>
 */
class NamespaceReader
{
    /**
     * Member function signature for nodetype-lookup
     * @see Parser::Parser
     */
    typedef Model::ElementRef (NamespaceReader::*MemberFunc)(const YAML::Node &);

public:
    /**
     * @brief Nodetypes of KEY_NODETYPE
     * @{
     */
    static const char *TYPE_NAMESPACE;
    static const char *TYPE_PRIMITIVE;
    static const char *TYPE_ENUM;
    static const char *TYPE_STRUCT;
    static const char *TYPE_CLASS;

    /** @} */

    /**
     * @brief Keys that appear in different sections
     * @{
     */
    static const char *KEY_NAME;
    static const char *KEY_SHORTNAME;
    static const char *KEY_NODETYPE;
    static const char *KEY_UNDERLYING;
    static const char *KEY_SUPER;
    static const char *KEY_TYPE;
    static const char *KEY_VALUE;
    static const char *KEY_RESULT;

    static const char *KEY_DOC;
    static const char *KEY_BRIEF;
    static const char *KEY_MORE;

    static const char *KEY_MEMBERS;
    static const char *KEY_PARAMS;
    static const char *KEY_FIELDS;
    static const char *KEY_VALUES;
    static const char *KEY_OPERATIONS;
    static const char *KEY_EVENTS;
    static const char *KEY_CONSTANTS;

    static const char *KEY_ID;
    /** @} */

    /**
     * @brief Keywords that are interpreted as boolean flags
     */
    static const char *FLAG_STATIC;
    //static const char *FLAG_SYNCHRONOUS;
    //static const char *FLAG_VALUETYPE;
    /** @} */

    /**
     * @brief Mappingtable of PRIMITIVE_* strings to resolve KEY_UNDERLYING
     */
    static const char *mPrimitiveMap[int(Model::Primitive::Underlying::_COUNT_)];

public:
    NamespaceReader(Model::NamespaceRef rootNamespace);

    /**
     * @brief Read an parse YAML file that conains namespace objects. All objects
     *        are placed into currently set root namespace.
     * @param filename  filename
     * @throw std::runtime_error on parse errors
     */
    void parseFile(std::istream& stream);

    /**
     * @brief Set current root namespace of object tree.
     * @param rootNamespace root namespace
     */
    void setRootNamespace(Model::NamespaceRef rootNamespace);

    /**
     * @brief Reset parsers type cache and namespace stack
     */
    void reset();

    /**
     * @brief Try to resolve all types contained in rootNamespace.
     *
     * This method replaces all UnresolvedType objects with correctly linked
     * Type() objects. Type lookup is done via mKnownTypes map.
     *
     * @param rootNamespace Namespace that will be scanned for unresolved types
     * @throw std::runtime_error in case of unresolvable types
     */
    void resolveTypesInNamespace(Model::NamespaceRef rootNamespace);

    /**
     * @brief Prints all registered types to stdout.
     */
    void listKnownTypes();


private:
    /**
     * @brief YAML file
     * @param fileName Filename of YAML file;
     * @return Root node
     * @throw std::runtime_error in case of YAML parser problems
     */
    virtual YAML::Node loadFile(std::istream& stream);

    /**
     * @brief Check if node[key] has a specific type.
     * @param node  YAML node
     * @param key   Key name
     * @param expectedType  YAML node type
     * @param mandatory if true, key must be found and must have the right type
     * @return true if node[key] has expectedType
     * @throw std::runtime_error if types don't match and mandatory is set
     */
    bool checkNode(const YAML::Node &node, const char *key,
                   YAML::NodeType::value expectedType = YAML::NodeType::Scalar, bool mandatory = false);

    /**
     * @brief Parse namespace members into rootNamespace starting at node.
     * @param node          YAML root node
     * @param rootNamespace Pointer to Namespace object that acts as root for found entries
     */
    void parseElements(const YAML::Node &node, Model::NamespaceRef rootNamespace);

    /**
     * @brief Parse TYPE_NAMESPACE section
     * @param node  YAML node that contains namespace definition
     * @return  Shared pointer to a filled Object
     * @throw  std::runtime_error on incomplete definition
     */
    Model::ElementRef parseNamespace(const YAML::Node &node);

    /**
     * @brief Parse TYPE_CLASS section
     * @param node  YAML node that contains class definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseClass(const YAML::Node &node);

    /**
     * @brief Parse TYPE_PRIMITIVE section
     * @param node  YAML node that contains primitive definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parsePrimitive(const YAML::Node &node);

    /**
     * @brief Parse TYPE_ENUM section
     * @param node  YAML node that contains enum definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseEnum(const YAML::Node &node);

    /**
     * @brief Parse TYPE_STRUCT section
     * @param node  YAML node that contains struct definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseStruct(const YAML::Node &node);

    /**
     * @brief Parse TYPE_CONSTANT definition
     * @param node  YAML node that contains a constant definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::ConstantRef parseClassConstant(const YAML::Node &node);

    /**
     * @brief Parse operation definition (entries of KEY_OPERATIONS inside TYPE_CLASS section);
     * @param node  YAML node that contains a single operation definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::OperationRef parseClassOperation(const YAML::Node &node);

    /**
     * @brief Parse event definition (entries of KEY_EVENTS inside TYPE_CLASS section)
     * @param node  YAML node that contains a single event definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::EventRef parseClassEvent(const YAML::Node &node);

    /**
     * @brief Parse a value definition (KEY_VALUE inside KEY_VALUES section)
     * @param node  YAML node that contains a value definition in KEY_VALUE
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Enum::ValueRef parseEnumValue(const YAML::Node &node);

    /**
     * @brief Parse parameter definition
     * @param node  YAML node that contains a parameter definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ParameterRef parseParameter(const YAML::Node &node);

    /**
     * @brief Parse type information
     * @param node  YAML node that contains a type definition after KEY_TYPE
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::TypeBaseRef parseType(const YAML::Node &node);

    /**
     * @brief Parse long and short name from node into identifiable.
     * @param node  YAML node with mandatory KEY_NAME and optional KEY_SHORTNAME
     * @param identifiable  Pointer to Identifiable object that will be named
     * @throw std::runtime_error if KEY_NAME was not found
     */
    void parseName(const YAML::Node &node, Model::IdentifiableRef identifiable);

    /**
     * @brief Parse documentation into identifiable
     * @param node  YAML node that contains KEY_DOC
     * @param identifiable  Pointer to Identifiable object
     */
    void parseDoc(const YAML::Node &node, Model::IdentifiableRef identifiable);

    /**
     * @brief Register type of member in map of known types.
     * @param member    Member object
     */
    void registerType(Model::ElementRef member);

    /**
     * @brief Try to resolve typeName to a ElementRef
     * @param typeName  Name of type relative to current namespace stack
     * @return Pointer to Element object | nullptr if not found
     */
    Model::ElementRef resolveTypeName(std::string typeName);

    /**
     * @brief Try to resolve UnresolvedType object into a new Type object
     * @param type  Type object that may be of type UnresolvedType or Type
     * @return new or existing Type object
     * @throw std::runtime_error if type is not resolvable or contains a base object
     */
    Model::TypeRef resolveType(Model::TypeBaseRef type);

    /**
     * @brief Try to resolve all types included in parameter
     * @param parameter Parameter
     * @throw std::runtime_error in case of an unresolvable type
     */
    void resolveParameterType(Model::ParameterRef parameter);

    /**
     * @brief Push current namespace onto namespace element stack
     * @param namespaceRoot
     */
    void startNamespace(Model::NamespaceRef namespaceRoot);

    /**
     * @brief Pop current namespace from namespace element stack
     */
    void endNamespace();

    /**
     * @brief Get fully qualified name of current namespace
     * @return  Fully qualified name of current namespace
     */
    std::string getCurrentNamespace();

    /**
     * @brief Create new Identifiable object of type T from name and doc info in node
     * @param node  YAML node that must contain at least a KEY_NAME
     * @return new Identifiable-based object of type T
     * @throw std::runtime_error in case T is not derived from Identifiable
     */
    template <typename T> std::shared_ptr<T> newIdentifiable(const YAML::Node &node)
    {
        std::shared_ptr<T> newMember(new T);

        Model::IdentifiableRef identifiable = std::dynamic_pointer_cast<Model::Identifiable>(newMember);
        if (identifiable)
        {
            parseName(node, identifiable);
            parseDoc(node, identifiable);
        }
        else
        {
            throw std::runtime_error("newIdentifiable(): Type is not an Identifiable!\n");
        }
        return newMember;
    }

private:
    Model::NamespaceRef mRootNamespace;
    std::map<std::string, MemberFunc> mParserMethods;
    std::map<std::string, Model::ElementRef> mKnownTypes;
    std::vector<std::string> mNamespaceElementStack;
};

} } } // namespace: Everbase::InterfaceCompiler::Components

