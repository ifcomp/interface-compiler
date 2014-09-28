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
    enum StyleAttribute
    {
        NAME_STYLE,
        NAME_DELIMITER,
        NAME_USE_SHORT,
        INDENT,
        TEXT_WRAP,
        _STYLE_ATTRIB_COUNT_
    };

    enum NameStyle
    {
        CAMELCASE,
        LOWERCASE,
        UPPERCASE,
        _NAME_STYLE_COUNT_
    };

    static const char *KEY_SECTION_TYPEMAP;
    static const char *KEY_TYPE_PRIMITIVES;
    static const char *KEY_TYPE_CONTAINERS;
    static const char *KEY_SECTION_STYLE;
    static const char *KEY_STYLE_CONTEXT_DEFAULT;

    static const char *styleAttributeKeys[StyleAttribute::_STYLE_ATTRIB_COUNT_];
    static const char *nameStyleKeys[NameStyle::_NAME_STYLE_COUNT_];

    static const char  TYPE_PLACEHOLDER;        ///< This placeholder is used for inner container types


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
     * @throw std::runtime_error if primitive could not be translated
     */
    std::string primitiveToLang(Model::PrimitivePtr primitive);

    /**
     * @brief Find language-specific output string for Container
     * @param container Pointer to Container object
     * @return language-specific keyword to represent Container
     * @throw std::runtime_error if container could not be translated
     */
    std::string containerToLang(Model::ContainerPtr container);

    /**
     * @brief Fetch NAME_STYLE config entry.
     * @param styleContext Style context enum entry to specify the config section
     * @return Configured NameStyle for styleContext
     */
    NameStyle configNameStyle(DomainObjectPtr styleContextObject);

    /**
     * @brief Print all registered style attributes to stdout.
     * @return List as string
     */
    std::string listKnownStyleAttributes();

    /**
     * Fetch style attribute from config.
     * @param styleAttribute Attribute to fetch
     * @param styleContext Style context to search attribute in
     * @return Attribute value
     */
    template <typename T> T configAttribute(StyleAttribute styleAttribute,
                                            DomainObjectPtr styleContextObject)
    {
        const YAML::Node &node = configValue(styleAttribute, styleContextObject);
        return node.as<T>();
    }

private:
    /**
     * @brief Fetch language-specific config value.
     *
     * This method tries to retrieve the config value specified by key.
     * If identifiable is given, the config value will be taken from
     * the according config section. If the section or the config key
     * are not defined, the key lookup takes place in the default section.
     *
     * @param styleAttribute Attribute entry to lookup in config
     * @param styleContext Enum entry that defines the config section in
     *                     which the key lookup takes place.
     * @return Config value
     * @throw std::runtime_error if key was not found in styleContext or default context
     * @see styleContextKeys
     */
    YAML::Node configValue(StyleAttribute styleAttribute, DomainObjectPtr styleContextObject);

private:
    YAML::Node mRootNode;
    std::map<std::string, std::string> mPrimitiveMap;
    std::map<std::string, std::string> mContainerMap;
};

} } // namespace Api::Parser

#endif // LANGCONFIGPARSER_HPP
