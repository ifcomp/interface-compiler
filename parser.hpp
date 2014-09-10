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

namespace Api {


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
     * @throws  std::exeption on incomplete definition
     */
    Model::NamespaceMemberPtr parseNamespace(const YAML::Node &node);

    /**
     * @brief Parse TYPE_CLASS section
     * @param node  YAML node that contains class definition
     * @return Shared pointer to a filled Object
     * @throws std::exeption on incomplete definition
     */
    Model::NamespaceMemberPtr parseClass(const YAML::Node &node);

    /**
     * @brief Parse TYPE_PRIMITIVE section
     * @param node  YAML node that contains primitive definition
     * @return Shared pointer to a filled Object
     * @throws std::exeption on incomplete definition
     */
    Model::NamespaceMemberPtr parsePrimitive(const YAML::Node &node);


    Model::NamespaceMemberPtr parseEnum(const YAML::Node &node);
    Model::NamespaceMemberPtr parseStruct(const YAML::Node &node);
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
     * @throws std::exception if KEY_NAME was not found
     */
    void parseName(const YAML::Node &node, Model::IdentifiablePtr identifiable);

    /**
     * @brief Parse documentation into identifiable
     * @param node  YAML node that contains KEY_DOC
     * @param identifiable
     */
    void parseDoc(const YAML::Node &node, Model::IdentifiablePtr identifiable);

    /**
     * @brief Check if node[key] has a specific type.
     * @param node          YAML node
     * @param key           Key name
     * @param expectedType  YAML node type
     * @param mandatory     Flag that marks key as mandatory
     * @return true if node[key] has expectedType
     * @throws std::exception if types don't match and mandatory is set
     */
    bool checkNode(const YAML::Node &node, const char *key,
                   YAML::NodeType::value expectedType = YAML::NodeType::Scalar, bool mandatory = false);

    /**
     * @brief Register type of member in map of known types.
     * @param member    Member object
     */
    void registerType(Model::NamespaceMemberPtr member);

    Model::NamespaceMemberPtr resolveType(std::string typeName);

    void startNamespace(Model::NamespacePtr namespaceRoot);
    void endNamespace();
    std::string getCurrentNamespace();

    bool resolveIdentifier(std::string identifier);

    template <typename T> std::shared_ptr<T> newIdentifiable(const YAML::Node &node);

private:
    YAML::Node mYamlConfig;
    Model::NamespacePtr mRootNamespace;
    std::map<std::string, MemberFunc> mParserMethods;
    std::map<std::string, Model::NamespaceMemberPtr> mKnownTypes;
    std::vector<std::string> mCurrentNamespaceElements;
};

} // namespace Api

#endif // PARSER_H
