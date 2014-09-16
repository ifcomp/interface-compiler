#include "generator/cppGenerator.hpp"

using namespace Api::Gen;



CppGenerator::CppGenerator(Api::Model::NamespacePtr rootNamespace, std::string outputDirectory)
    : Generator(rootNamespace, outputDirectory)
{

}
