#ifndef FORMATTER_HPP
#define FORMATTER_HPP

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

class Formatter
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

protected:
    Formatter(std::string configFilename);

    virtual std::string name(Model::IdentifiablePtr identifiable);                      // c++: MeinAuto, c: mein_auto_t
    virtual std::string type(Model::TypePtr type, bool fullyQualified = false);         // c++: std::shared_ptr<Everbase::MeinAuto>, c: mein_auto_t
    virtual std::string classType(Model::ClassPtr classPtr);                            // if (classType == ABSTRACT) return "virtual";
    virtual std::string doc(Model::Documentation doc);                                  // /**\n  *....
    virtual std::string param(Model::ParameterPtr param);                               // c++ std::shared_ptr<Everbase::MeinAuto> autoMobil

    virtual std::string operation(Model::OperationPtr operation) = 0;
    virtual std::string event(Model::OperationPtr event) = 0;

protected:
    std::string mConfigFilename;
    Parser::LangConfigParser mParser;

};

typedef std::shared_ptr<Formatter>  FormatterPtr;

} } // namespace Api::Gen

#endif // FORMATTER_HPP