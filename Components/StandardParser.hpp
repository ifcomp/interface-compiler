#pragma once

#include "Parser.hpp"
#include "Model.hpp"

#include <yaml-cpp/yaml.h>
#include <functional>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class StandardParser : public Parser
{
public:    
    /**
     * Member function signature for nodetype-lookup
     * @see Parser::Parser
     */
    typedef Model::ElementRef (StandardParser::*MemberFunc)(const YAML::Node &, const Model::RootRef &) const;

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
    static const char *FLAG_SYNCHRONOUS;
    static const char *FLAG_VALUETYPE;
    /** @} */

    /**
     * @brief Mappingtable of PRIMITIVE_* strings to resolve KEY_UNDERLYING
     */
    static const char *_primitiveMap[int(Model::Primitive::Underlying::_COUNT_)];

public:
    virtual Model::RootRef execute ( std::istream& input ) const override;

    /**
     * @brief Read an parse YAML file that conains namespace objects. All objects
     *        are placed into a new root object.
     * @param stream Stream with YAML data as input
     * @return Root node of object tree
     * @throw std::runtime_error on parse errors
     */
    Model::RootRef parseFile(std::istream& stream) const;

    /**
     * @brief Resolve types in object tree.
     * @param root Root of object tree
     */
    static void resolve(const Model::RootRef &root);

private:
    /**
     * @brief Check if node[key] has a specific type.
     * @param node  YAML node
     * @param key   Key name
     * @param expectedType  YAML node type
     * @param mandatory if true, key must be found and must have the right type
     * @return true if node[key] has expectedType
     * @throw std::runtime_error if types don't match and mandatory is set
     */
    static bool checkNode(const YAML::Node &node, const char *key,
                   YAML::NodeType::value expectedType = YAML::NodeType::Scalar,
                   bool mandatory = false);

    static std::string getNamespace(const Model::IdentifiableRef &identifiable);

    /**
     * @brief Parse namespace members into rootNamespace starting at node.
     * @param node          YAML root node
     * @param rootNamespace Pointer to Namespace object that acts as root for found entries
     */
    void parseElements(const YAML::Node &node, const Model::NamespaceRef &rootNamespace, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_NAMESPACE section
     * @param node  YAML node that contains namespace definition
     * @return  Shared pointer to a filled Object
     * @throw  std::runtime_error on incomplete definition
     */
    Model::ElementRef parseNamespace(const YAML::Node &node, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_CLASS section
     * @param node  YAML node that contains class definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseClass(const YAML::Node &node, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_PRIMITIVE section
     * @param node  YAML node that contains primitive definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parsePrimitive(const YAML::Node &node, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_ENUM section
     * @param node  YAML node that contains enum definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseEnum(const YAML::Node &node, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_STRUCT section
     * @param node  YAML node that contains struct definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ElementRef parseStruct(const YAML::Node &node, const Model::RootRef &root) const;

    /**
     * @brief Parse TYPE_CONSTANT definition
     * @param node  YAML node that contains a constant definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::ConstantRef parseClassConstant(const YAML::Node &node) const;

    /**
     * @brief Parse operation definition (entries of KEY_OPERATIONS inside TYPE_CLASS section);
     * @param node  YAML node that contains a single operation definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::OperationRef parseClassOperation(const YAML::Node &node) const;

    /**
     * @brief Parse event definition (entries of KEY_EVENTS inside TYPE_CLASS section)
     * @param node  YAML node that contains a single event definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Class::EventRef parseClassEvent(const YAML::Node &node) const;

    /**
     * @brief Parse a value definition (KEY_VALUE inside KEY_VALUES section)
     * @param node  YAML node that contains a value definition in KEY_VALUE
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::Enum::ValueRef parseEnumValue(const YAML::Node &node) const;

    /**
     * @brief Parse parameter definition
     * @param node  YAML node that contains a parameter definition
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::ParameterRef parseParameter(const YAML::Node &node) const;

    /**
     * @brief Parse type information
     * @param node  YAML node that contains a type definition after KEY_TYPE
     * @return Shared pointer to a filled Object
     * @throw std::runtime_error on incomplete definition
     */
    Model::TypeBaseRef parseType(const YAML::Node &node) const;

    /**
     * @brief Parse long and short name from node into identifiable.
     * @param node  YAML node with mandatory KEY_NAME and optional KEY_SHORTNAME
     * @param identifiable  Pointer to Identifiable object that will be named
     * @throw std::runtime_error if KEY_NAME was not found
     */
    static void parseName(const YAML::Node &node, const Model::IdentifiableRef &identifiable);

    /**
     * @brief Parse documentation into identifiable
     * @param node  YAML node that contains KEY_DOC
     * @param identifiable  Pointer to Identifiable object
     */
    static void parseDoc(const YAML::Node &node, const Model::IdentifiableRef &identifiable);

    /**
     * @brief Find Element equal to name.
     * @param parent Parent element to search in
     * @param name Name of the element to seach for
     * @param matchSiblings If true the siblings of parent are compared against name.
     * @return
     */
    static Model::ElementRef findElement(const Model::ElementRef &parent, std::string name, bool matchSiblings = true);

    /**
     * @brief Try to resolve typeName to a ElementRef
     * @param typeName  Name of type relative to current namespace stack
     * @return Pointer to Element object | nullptr if not found
     */
    static Model::ElementRef resolveTypeName(std::string typeName, const Model::RootRef &root);

    /**
     * @brief Try to resolve UnresolvedType object into a new Type object
     * @param type  Type object that may be of type UnresolvedType or Type
     * @return new or existing Type object
     * @throw std::runtime_error if type is not resolvable or contains a base object
     */
    static Model::TypeRef resolveType(const Model::TypeBaseRef &type, const Model::RootRef &root);

    /**
     * @brief Try to resolve all types included in parameter
     * @param parameter Parameter
     * @throw std::runtime_error in case of an unresolvable type
     */
    static void resolveParameterType(const Model::ParameterRef &parameter, const Model::RootRef &root);

    /**
     * @brief Try to resolve all types contained in rootNamespace.
     *
     * This method replaces all UnresolvedType objects with correctly linked
     * Type() objects. Type lookup is done via mKnownTypes map.
     *
     * @param rootNamespace Namespace that will be scanned for unresolved types
     * @throw std::runtime_error in case of unresolvable types
     */
    static void resolveTypesInNamespace(const Model::NamespaceRef &rootNamespace, const Model::RootRef &root);

    /**
     * @brief Create new Identifiable object of type T from name and doc info in node
     * @param node  YAML node that must contain at least a KEY_NAME
     * @return new Identifiable-based object of type T
     * @throw std::runtime_error in case T is not derived from Identifiable
     */
    template <typename T> static std::shared_ptr<T> newIdentifiable(const YAML::Node &node)
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

};

} } } // namespace: Everbase::InterfaceCompiler::Components
