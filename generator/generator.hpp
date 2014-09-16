#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "generator/formatter.hpp"
#include "model/namespace.hpp"
#include "model/resolvedType.hpp"

namespace Api { namespace Gen {

class Generator
{
protected:
    Generator(Model::NamespacePtr rootNamespace, std::string outputDirectory);

    void setOutputDirectory(std::string outputDirectory);

    /**
     * @brief Iterate through all included namespaces and call writeHeader() on each one.
     * @param glue  If false, generate header files; if true, generate glue code
     */
    void generate(bool glue = false);

    virtual void writeHeader() = 0;
    virtual void writeGlue() = 0;

    std::string writeType(Model::ResolvedTypePtr type);

protected:
    FormatterPtr mFormatter;

private:
    Model::NamespacePtr mRootNamespace;
    std::string mOutputDirectory;
};

} } // namespace Api::Gen

#endif // GENERATOR_HPP
