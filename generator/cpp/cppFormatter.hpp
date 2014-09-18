#ifndef CPPFORMATTER_HPP
#define CPPFORMATTER_HPP

#include "generator/formatter.hpp"

namespace Api { namespace Gen {

class CppFormatter : public Formatter
{
public:
    CppFormatter(std::string configFilename);

    std::string param(Model::ParameterPtr param);
    std::string result(Model::ParameterPtr param, bool fullyQualified);
    std::string operation(Model::OperationPtr operation);
    std::string event(Model::EventPtr event);

};

} } // namespace Api::Gen

#endif // CPPFORMATTER_HPP
