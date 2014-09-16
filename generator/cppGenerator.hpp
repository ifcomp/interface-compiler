#ifndef CPPGENERATOR_HPP
#define CPPGENERATOR_HPP

#include "generator/generator.hpp"

namespace Api { namespace Gen {

class CppGenerator : public Generator
{
public:
    CppGenerator(Model::NamespacePtr rootNamespace, std::string outputDirectory);

};

} } // namespace Api::Gen

#endif // CPPGENERATOR_HPP
