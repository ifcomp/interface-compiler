#ifndef CPPHEADERS_HPP
#define CPPHEADERS_HPP

#include "formatter/generator.hpp"

namespace Api { namespace Gen {

class CppHeaders : public Generator
{
public:
    CppHeaders(Model::NamespacePtr rootNamespace, std::string outputDirectory);

    virtual void generate();

private:
    void iterateNamespace(Model::NamespacePtr namespacePtr);

};

} } // namespace Api::Gen

#endif // CPPHEADERS_HPP
