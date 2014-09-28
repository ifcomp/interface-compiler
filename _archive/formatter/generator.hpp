#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "formatter/formatter.hpp"
#include "model/namespace.hpp"
#include "model/resolvedType.hpp"

namespace Api { namespace Gen {

class Generator
{
public:
    void setOutputDirectory(std::string outputDirectory);
    virtual void generate() = 0;

protected:
    Generator(Model::NamespacePtr rootNamespace, std::string outputDirectory);

protected:
    FormatterPtr mFormatter;
    Model::NamespacePtr mRootNamespace;
    std::string mOutputDirectory;
};

} } // namespace Api::Gen

#endif // GENERATOR_HPP
