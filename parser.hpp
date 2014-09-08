#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <model/namespace.hpp>

namespace Api {

class Parser
{
public:
    Parser(Model::NamespacePtr rootNamespace);

    bool parseFile(std::string filename);
    void setRootNamespace(Model::NamespacePtr rootNamespace);

private:
    void parseNode(const YAML::Node &node);

    void parseNamespace(YAML::Node &node);
    bool resolveIdentifier(std::string identifier);

private:
    YAML::Node mYamlConfig;
    Model::NamespacePtr mRootNamespace;
};

} // namespace Api

#endif // PARSER_H
