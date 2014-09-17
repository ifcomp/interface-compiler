#ifndef LANGCONFIGPARSER_HPP
#define LANGCONFIGPARSER_HPP

#include "parser/yamlParser.hpp"
#include "model/primitive.hpp"
#include "model/container.hpp"

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

public:
    LangConfigParser(std::string configFilename);

    /**
     * @brief Validate typemap-entries in config file
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

    std::string containerToLang(Model::ContainerPtr container);

private:
    YAML::Node mRootNode;
    std::map<std::string, std::string> mPrimitiveMap;
    std::map<std::string, std::string> mContainerMap;
};

} } // namespace Api::Parser

#endif // LANGCONFIGPARSER_HPP
