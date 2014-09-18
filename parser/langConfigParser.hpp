#ifndef LANGCONFIGPARSER_HPP
#define LANGCONFIGPARSER_HPP

#include "parser/yamlParser.hpp"
#include "model/primitive.hpp"
#include "model/container.hpp"
#include "model/resolvedType.hpp"

namespace Api { namespace Parser {

class LangConfigParser : public YamlParser
{
public:
    static const char* KEY_TYPEMAP;
    static const char* KEY_TYPE_PRIMITIVES;
    static const char* KEY_TYPE_CONTAINERS;

    static const char* KEY_STYLE;
    static const char* KEY_STYLE_CONTEXT;
    static const char* KEY_STYLE_NAMESTYLE;
    static const char* KEY_STYLE_NAMEDELIMITER;
    static const char* KEY_STYLE_NAMEUSESHORT;

    static const char* styleContextKeys[];

    static const char  TYPE_PLACEHOLDER;

public:
    /**
     * @brief Constructor that directly loads config file
     * @param configFilename YAML file that contains language-specific config
     */
    LangConfigParser(std::string configFilename);

    /**
     * @brief Parse typemap-entries of config file into maps.
     * @note Make sure you call this method before using primitiveToLang()
     *       or containerToLang()
     * @throws std::runtime_error on parse error
     */
    void parseTypeMap();

    /**
     * @brief Find language-specific output string for Primitive
     * @param primitive Pointer to Primitive object
     * @return language-specific keyword to represent Primitive
     */
    std::string primitiveToLang(Model::PrimitivePtr primitive);

    /**
     * @brief Find language-specific output string for Container
     * @param container Pointer to Container object
     * @return language-specific keyword to represent Container
     */
    std::string containerToLang(Model::ContainerPtr container);

    /**
     * @brief Resolve Parameter to language-specific output string
     * @param parameter Parameter object
     * @return Language-specific token
     */
    std::string containerTypeToLang(Model::TypePtr type);

private:
    YAML::Node mRootNode;
    std::map<std::string, std::string> mPrimitiveMap;
    std::map<std::string, std::string> mContainerMap;
};

} } // namespace Api::Parser

#endif // LANGCONFIGPARSER_HPP
