#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <stack>
#include "model/identifiable.hpp"
#include "model/namespace.hpp"
#include "model/resolvedType.hpp"
#include "model/class.hpp"
#include "model/parameter.hpp"
#include "model/documentation.hpp"
#include "model/operation.hpp"
#include "model/event.hpp"
#include "parser/langConfigParser.hpp"

namespace Api { namespace Gen {

/**
 * @brief Base class for formatting language-specific output files.
 * @author Gunther Lemm <lemm@silpion.de>
 */
class Formatter
{
//public:
//    static const char* KEY_TYPEMAP;
//    static const char* KEY_TYPE_PRIMITIVES;
//    static const char* KEY_TYPE_CONTAINERS;

//    static const char* KEY_STYLE;
//    static const char* KEY_STYLE_CONTEXT;
//    static const char* KEY_STYLE_NAMESTYLE;
//    static const char* KEY_STYLE_NAMEDELIMITER;
//    static const char* KEY_STYLE_NAMEUSESHORT;

//    static const char* styleContextKeys[];

public:
    /**
     * @brief Format a name token without indentation.
     * @note Converts object name to something like this: c++: MyCar, c: my_car
     * @param identifiable Pointer to Identifiable object as source of name
     * @return Formatted name string
     */
    virtual std::string name(Model::IdentifiablePtr identifiable);

    /**
     * @brief Format a type token without indentation.
     * @note Converts type info to something like this: c++: std::shared_ptr<Everbase::MyCar>, c: my_car_t
     * @param type Pointer to ResolvedType object
     * @param fullyQualified Add namespace if true
     * @return Formatted type string
     * @throw std::runtime_error in case of wrong type
     */
    virtual std::string type(Model::TypePtr type, bool fullyQualified = false);

    /**
     * @brief Format indented doxygen documentation entry including parameter documentation.
     * @param doc Pointer to Documentation object
     * @return Formatted documentation for doc
     */
    virtual std::string doc(Model::DocumentationPtr doc);

    /**
     * @brief Format parameter in a language-specific way without indentation.
     * @note This method is implemented by derived class. Its output may look like:
     *       c++: std::shared_ptr<Everbase::MyCar> myCar, c: my_car_t my_car
     * @param param Pointer to Parameter object
     * @return Formatted parameter
     */
    virtual std::string param(Model::ParameterPtr param) = 0;

    /**
     * @brief Format result in a language-specific way without indentation.
     * @note This method is implemented by derived class.
     * @param param Pointer to Parameter object
     * @param fullyQualified Add namespace if true
     * @return Formatted result string
     */
    virtual std::string result(Model::ParameterPtr param, bool fullyQualified = false) = 0;

    /**
     * @brief Format operation in a language-specific way including indentation.
     * @note This method is implemented by derived class.
     * @param operation Pointer to Operation object
     * @return Formatted operation including documentation, result, function name & params.
     */
    virtual std::string operation(Model::OperationPtr operation) = 0;


    /**
     * @brief Format event in a language-specific way including indentation.
     * @note This method is implemented by derived class.
     * @param event Pointer to Event object
     * @return Formatted event string
     */
    virtual std::string event(Model::EventPtr event) = 0;

    /**
     * @brief Start section with incremented indent.
     * @param indent Number of additional indent whitespaces
     */
    void beginIndent(uint32_t indentCount);

    /**
     * @brief Start section with an additional indent as defined in identifiable's config.
     * @param identifiable Pointer to Identifiable to determine config section
     */
    void beginIndent(Model::IdentifiablePtr identifiable);

    /**
     * @brief Decrement indentation by last incremented step.
     */
    void endIndent();

    /**
     * @brief Get current indent.
     * @return Number of indentation chars
     */
    uint32_t indentCount();

    /**
     * @brief Return mCurrentIndent whitespaces
     * @return Whitespace string
     */
    std::string indent();

    /**
     * @brief Resolve Parameter to language-specific output string
     * @param parameter Parameter object
     * @return Language-specific token
     */
    std::string containerTypeToLang(Model::TypePtr type, bool fullyQualified);

protected:
    Formatter(std::string configFilename);

    /**
     * @brief Convert name into styled name by looking at the language-specific config.
     * @param name Unstyled name
     * @param styleContext Configuration context from which the config will be fetched
     * @return Styled output string
     */
    virtual std::string styleToken(std::string name, Parser::LangConfigParser::StyleContext styleContext =
                                                     Parser::LangConfigParser::StyleContext::DEFAULT);

    /**
     * @brief Resolve namespace of identifiable object by traversing parent pointers.
     * @param identifiable Pointer to Identifiable object
     * @return Namespace string
     */
    virtual std::string objectNamespace(Model::IdentifiablePtr identifiable);

    /**
     * @brief Add newlines to text as defined in language-specific config
     * @param text Text
     * @param styleContext Configuration context from which the config will be fetched
     * @return Wrapped text
     */
    std::string wrapText(std::string text, Parser::LangConfigParser::StyleContext styleContext =
                                           Parser::LangConfigParser::StyleContext::DEFAULT,
                         std::string linePrefix = "");

protected:
    Parser::LangConfigParser mParser;

private:
    uint32_t mCurrentIndent;
    std::stack<uint32_t> mIndentStack;
};

typedef std::shared_ptr<Formatter>  FormatterPtr;

} } // namespace Api::Gen

#endif // FORMATTER_HPP
