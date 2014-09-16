#ifndef CPPGENERATOR_HPP
#define CPPGENERATOR_HPP

#include "generator/generator.hpp"

namespace Api { namespace Gen {

class CppGenerator : public Generator
{
public:
    CppGenerator(Model::NamespacePtr rootNamespace, std::string outputDirectory);

    void writeHeader();
    void writeGlue();

};

} } // namespace Api::Gen

#endif // CPPGENERATOR_HPP
