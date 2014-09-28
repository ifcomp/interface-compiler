#ifndef CPPFORMATTER_HPP
#define CPPFORMATTER_HPP

#include "formatter/formatter.hpp"

namespace Api { namespace Gen {

class CppFormatter : public Formatter
{
public:
    CppFormatter(std::string configFilename);

    virtual void param(std::ostream &stream, Model::ParameterPtr param);
    virtual void result(std::ostream &stream, Model::ParameterPtr param, bool fullyQualified);
    virtual void operation(std::ostream &stream, Model::OperationPtr operation);
    virtual void event(std::ostream &stream, Model::EventPtr event);

};

} } // namespace Api::Gen

#endif // CPPFORMATTER_HPP
