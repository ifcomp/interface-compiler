#ifndef YAMLPARSER_HPP
#define YAMLPARSER_HPP

#include <yaml-cpp/yaml.h>

namespace Api { namespace Parser {

/**
 * @brief Base class for building YAML parsers.
 * @author Gunther Lemm <lemm@silpion.de>
 */
class YamlParser
{    
protected:
    YamlParser();

    /**
     * @brief YAML file
     * @param fileName Filename of YAML file;
     * @return Root node
     * @throw std::runtime_error in case of YAML parser problems
     */
    virtual YAML::Node loadFile(std::string fileName);

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
};

} } // namespace Api::Parser

#endif // YAMLPARSER_HPP
