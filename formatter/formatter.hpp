#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <ostream>

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
public:
    /**
     * @brief Format a name token without indentation.
     * @note Converts object name to something like this: c++: MyCar, c: my_car
     * @param identifiable Pointer to Identifiable object as source of name
     * @return Formatted name string
     */
    virtual void name(std::ostream &stream, Model::IdentifiablePtr identifiable);

    /**
     * @brief Format a type token without indentation.
     * @note Converts type info to something like this: c++: std::shared_ptr<Everbase::MyCar>, c: my_car_t
     * @param type Pointer to ResolvedType object
     * @param fullyQualified Add namespace if true
     * @return Formatted type string
     * @throw std::runtime_error in case of wrong type
     */
    virtual void type(std::ostream &stream, Model::TypePtr type, bool fullyQualified = false);

    /**
     * @brief Format indented doxygen documentation entry including parameter documentation.
     * @param doc Pointer to Documentation object
     * @return Formatted documentation for doc
     */
    virtual void doc(std::ostream &stream, Model::DocumentationPtr doc);

    /**
     * @brief Format parameter in a language-specific way without indentation.
     * @note This method is implemented by derived class. Its output may look like:
     *       c++: std::shared_ptr<Everbase::MyCar> myCar, c: my_car_t my_car
     * @param param Pointer to Parameter object
     * @return Formatted parameter
     */
    virtual void param(std::ostream &stream, Model::ParameterPtr param) = 0;

    /**
     * @brief Format result in a language-specific way without indentation.
     * @note This method is implemented by derived class.
     * @param param Pointer to Parameter object
     * @param fullyQualified Add namespace if true
     * @return Formatted result string
     */
    virtual void result(std::ostream &stream, Model::ParameterPtr param, bool fullyQualified = false) = 0;

    /**
     * @brief Format operation in a language-specific way including indentation.
     * @note This method is implemented by derived class.
     * @param operation Pointer to Operation object
     * @return Formatted operation including documentation, result, function name & params.
     */
    virtual void operation(std::ostream &stream, Model::OperationPtr operation) = 0;

    /**
     * @brief Format event in a language-specific way including indentation.
     * @note This method is implemented by derived class.
     * @param event Pointer to Event object
     * @return Formatted event string
     */
    virtual void event(std::ostream &stream, Model::EventPtr event) = 0;

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
    virtual void styleToken(std::ostream &stream, std::string name, DomainObjectPtr styleContextObject = nullptr);

    /**
     * @brief Resolve namespace of identifiable object by traversing parent pointers.
     * @param identifiable Pointer to Identifiable object
     * @return Namespace string
     */
    virtual void objectNamespace(std::ostream &stream, DomainObjectPtr identifiable);

protected:
    Parser::LangConfigParser mParser;
};

typedef std::shared_ptr<Formatter>  FormatterPtr;

} } // namespace Api::Gen

#endif // FORMATTER_HPP
