#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "model/identifiable.hpp"
#include "model/namespace.hpp"
#include "model/resolvedType.hpp"
#include "model/class.hpp"
#include "model/parameter.hpp"
#include "model/documentation.hpp"
#include "model/operation.hpp"
#include "model/event.hpp"

namespace Api { namespace Gen {

class Formatter
{
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
    YAML::Node mYamlConfig;
    std::map<std::string, std::string> mTypeMap;            ///< type mapping from object model to language specific type
};

typedef std::shared_ptr<Formatter>  FormatterPtr;

} } // namespace Api::Gen

#endif // FORMATTER_HPP
