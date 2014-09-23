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
    enum StyleContext
    {
        DEFAULT,
        CLASS,
        CONSTANT,
        CONTAINER,
        DOC,
        ENUM,
        EVENT,
        NAMESPACE,
        OPERATION,
        PARAMETER,
        PRIMITIVE,
        STRUCT,
        TYPE,
        VALUE,
        _STYLE_CONTEXT_COUNT_
    };

    enum NameStyle
    {
        CAMELCASE,
        LOWERCASE,
        UPPERCASE,
        _NAME_STYLE_COUNT_
    };

    static const char* KEY_TYPEMAP;
    static const char* KEY_TYPE_PRIMITIVES;
    static const char* KEY_TYPE_CONTAINERS;

    static const char* KEY_STYLE;
    static const char* KEY_STYLE_CONTEXT;
    static const char* KEY_STYLE_NAMESTYLE;
    static const char* KEY_STYLE_NAMEDELIMITER;
    static const char* KEY_STYLE_NAMEUSESHORT;

    static const char* styleContextKeys[int(StyleContext::_STYLE_CONTEXT_COUNT_)];
    static const char* nameStyleKeys[int(NameStyle::_NAME_STYLE_COUNT_)];

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
//    std::string containerTypeToLang(Model::TypePtr type);

    /**
     * @brief Fetch language-specific config value.
     *
     * This method tries to retrieve the config value specified by key.
     * If identifiable is given, the config value will be taken from
     * the according config section. If the section or the config key
     * are not defined, the key lookup takes place in the default section.
     *
     * @param key Config key
     * @param identifiable Pointer to Identifiable object that defines the config section
     *        in which the key lookup takes place.
     * @return Config value
     */
    YAML::Node configValue(std::string key, Model::IdentifiablePtr identifiable = nullptr);

    /**
     * @brief Fetch language-specific config value.
     * @param key Config key
     * @param styleContext Key of context section
     * @return Config value
     */
    YAML::Node configValue(std::string key, LangConfigParser::StyleContext styleContext);


    NameStyle configNameStyle(Model::IdentifiablePtr identifiable = nullptr);
    NameStyle configNameStyle(LangConfigParser::StyleContext styleContext);

    bool configUseShortNames(LangConfigParser::StyleContext styleContext);
    std::string configNameDelimiter(LangConfigParser::StyleContext styleContext);

    /**
     * @brief Map Identifiable object to StyleContext
     * @param identifiable Pointer to Identifiable object
     * @return StyleContext of identifiable (DEFAULT if not found)
     */
    StyleContext identifiableToStyleContext(Model::IdentifiablePtr identifiable = nullptr);

private:
    YAML::Node mRootNode;
    std::map<std::string, std::string> mPrimitiveMap;
    std::map<std::string, std::string> mContainerMap;
};

} } // namespace Api::Parser

#endif // LANGCONFIGPARSER_HPP
