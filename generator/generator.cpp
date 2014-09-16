#include "generator.hpp"

using namespace Api::Gen;

Generator::Generator(Api::Model::NamespacePtr rootNamespace, std::string outputDirectory)
    : mRootNamespace(rootNamespace)
    , mOutputDirectory(outputDirectory)
{
}

void Generator::setOutputDirectory(std::string outputDirectory)
{

}


void Generator::generate(bool glue)
{
    for (auto member : mRootNamespace->members())
    {

    }
}
