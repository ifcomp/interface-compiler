#ifndef CPPFORMATTER_HPP
#define CPPFORMATTER_HPP

#include "generator/formatter.hpp"

namespace Api { namespace Gen {

class CppFormatter : public Formatter
{
public:
    CppFormatter(std::string typemapFilename, std::string configFilename);

    std::string operation(Model::OperationPtr operation) { return ""; };
    std::string event(Model::OperationPtr event) { return ""; };

};

} } // namespace Api::Gen

#endif // CPPFORMATTER_HPP
