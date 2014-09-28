#pragma once

#include <yaml-cpp/yaml.h>
#include <istream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

/**
 * @brief Base class for building YAML parsers.
 * @author Gunther Lemm <lemm@silpion.de>
 */
class YamlReader
{    
protected:
    YamlReader();

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
};

} } } // namespace: Everbase::InterfaceCompiler::Components
