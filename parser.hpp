#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <functional>
#include <map>
#include <yaml-cpp/yaml.h>
#include <model/namespace.hpp>
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
#include "model/unresolvedType.hpp"
#include "model/resolvedType.hpp"

namespace Api {

/**
 * YAML parser class for namespace objects
 * @author Gunther Lemm <lemm@silpion.de>
 */
class Parser
{
    typedef Model::NamespaceMemberPtr (Parser::*MemberFunc)(const YAML::Node &);

public:
    static const char *TYPE_NAMESPACE;
    static const char *TYPE_CLASS;
    static const char *TYPE_PRIMITIVE;
    static const char *TYPE_ENUM;
    static const char *TYPE_STRUCT;
    static const char *TYPE_CONTAINER;

    static const char *CONTAINER_VECTOR;
    static const char *CONTAINER_LIST;
    static const char *CONTAINER_SET;

    static const char *KEY_NAME;
    static const char *KEY_SHORTNAME;
    static const char *KEY_NODETYPE;
    static const char *KEY_CONTAINER_TYPE;
    static const char *KEY_INHERITS;
    static const char *KEY_TYPE;
    static const char *KEY_VALUE;
    static const char *KEY_RETURN;

    static const char *KEY_DOC;
    static const char *KEY_BRIEF;
    static const char *KEY_MORE;

    static const char *KEY_MEMBERS;
    static const char *KEY_PARAMS;
    static const char *KEY_FIELDS;
    static const char *KEY_VALUES;
    static const char *KEY_OPERATIONS;
    static const char *KEY_EVENTS;

    static const char *FLAG_STATIC;
    static const char *FLAG_SYNCHRONOUS;
    static const char *FLAG_VALUETYPE;

public:
    Parser(Model::NamespacePtr rootNamespace);

    /**
     * @brief Read an parse YAML file that conains namespace objects. All objects
     *        are placed into currently set root namespace.
     * @param filename  filename
     */
    void parseFile(std::string filename);

    /**
     * @brief Set current root namespace of object tree.
     * @param rootNamespace root namespace
     */
    void setRootNamespace(Model::NamespacePtr rootNamespace);

    /**
     * @brief Reset parsers type cache and namespace stack
     */
    void reset();

    /**
     * @brief Try to resolve all types contained in rootNamespace.
     *
     * This method replaces all UnresolvedType objects with correctly linked
     * ResolvedType() objects. Type lookup is done via mKnownTypes map.
     *
     * @param rootNamespace Namespace that will be scanned for unresolved types
     * @throw std::runtime_error in case of unresolvable types
     */
    void resolveTypesInNamespace(Model::NamespacePtr rootNamespace);

    /**
     * @brief Prints all registered types to stdout.
     */
    void listKnownTypes();


private:
    /**
     * @brief Parse namespace members into rootNamespace starting at node.
     * @param node          YAML root node
     * @param rootNamespace Pointer to Namespace object that acts as root for found entries
     */
    void parseNamespaceMembers(const YAML::Node &node, Model::NamespacePtr rootNamespace);

    /**
     * @brief Parse TYPE_NAMESPACE section
     * @param node  YAML node that contains namespace definition
     * @return  Shared pointer to a filled Object
     * @throw  std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parseNamespace(const YAML::Node &node);

    /**
     * @brief Parse TYPE_CLASS section
     * @param node  YAML node that contains class definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parseClass(const YAML::Node &node);

    /**
     * @brief Parse TYPE_PRIMITIVE section
     * @param node  YAML node that contains primitive definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parsePrimitive(const YAML::Node &node);

    /**
     * @brief Parse TYPE_ENUM section
     * @param node  YAML node that contains enum definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parseEnum(const YAML::Node &node);

    /**
     * @brief Parse TYPE_STRUCT section
     * @param node  YAML node that contains struct definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parseStruct(const YAML::Node &node);

    /**
     * @brief Parse TYPE_PRIMITIVE section
     * @param node  YAML node that contains primitive definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::NamespaceMemberPtr parseContainer(const YAML::Node &node);

    Model::OperationPtr parseOperation(const YAML::Node &node);
    Model::EventPtr parseEvent(const YAML::Node &node);
    Model::ValuePtr parseValue(const YAML::Node &node);
    Model::ParameterPtr parseParameter(const YAML::Node &node);
    Model::TypePtr parseType(const YAML::Node &node);

    /**
     * @brief Parse long and short name from node into identifiable.
     * @param node  YAML node with mandatory KEY_NAME and optional KEY_SHORTNAME
     * @param identifiable  Pointer to Identifiable object that will be named
     * @throw std::runtime_error if KEY_NAME was not found
     */
    void parseName(const YAML::Node &node, Model::IdentifiablePtr identifiable);

    /**
     * @brief Parse documentation into identifiable
     * @param node  YAML node that contains KEY_DOC
     * @param identifiable  Pointer to Identifiable object
     */
    void parseDoc(const YAML::Node &node, Model::IdentifiablePtr identifiable);

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
     * @brief Register type of member in map of known types.
     * @param member    Member object
     */
    void registerType(Model::NamespaceMemberPtr member);

    /**
     * @brief Try to resolve typeName to a NamespaceMemberPtr
     * @param typeName  Name of type relative to current namespace stack
     * @return Pointer to NamespaceMember object | nullptr if not found
     */
    Model::NamespaceMemberPtr resolveTypeName(std::string typeName);

    /**
     * @brief Try to resolve UnresolvedType object into a new ResolvedType object
     * @param type  Type object that may be of type UnresolvedType or ResolvedType
     * @return new or existing ResolvedType object
     * @throw std::runtime_error if type is not resolvable or contains a base object
     */
    Model::ResolvedTypePtr resolveType(Model::TypePtr type);

    /**
     * @brief Try to resolve all types included in parameter
     * @param parameter Parameter
     * @throw std::runtime_error in case of an unresolvable type
     */
    void resolveParameterType(Model::ParameterPtr parameter);

    /**
     * @brief Push current namespace onto namespace element stack
     * @param namespaceRoot
     */
    void startNamespace(Model::NamespacePtr namespaceRoot);

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
    template <typename T> std::shared_ptr<T> newIdentifiable(const YAML::Node &node);

private:
    Model::NamespacePtr mRootNamespace;
    std::map<std::string, MemberFunc> mParserMethods;
    std::map<std::string, Model::NamespaceMemberPtr> mKnownTypes;
    std::vector<std::string> mNamespaceElementStack;
};

} // namespace Api

#endif // PARSER_H
