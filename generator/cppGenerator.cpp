#include "generator/cppGenerator.hpp"
#include "generator/cppFormatter.hpp"

using namespace Api::Gen;
using namespace std;

CppGenerator::CppGenerator(Api::Model::NamespacePtr rootNamespace, std::string outputDirectory)
    : Generator(rootNamespace, outputDirectory)
{
    mFormatter = make_shared<CppFormatter>("../api-generator/yaml/cpp/config.yaml");
}

void CppGenerator::writeHeader()
{

}

void CppGenerator::writeGlue()
{

}
