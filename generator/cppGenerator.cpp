#include "generator/cppGenerator.hpp"
#include "generator/cppFormatter.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace std;

CppGenerator::CppGenerator(Api::Model::NamespacePtr rootNamespace, std::string outputDirectory)
    : Generator(rootNamespace, outputDirectory)
{
    mFormatter = make_shared<CppFormatter>("../api-generator/yaml/cpp/config.yaml");
}

void CppGenerator::writeHeader()
{
    cout << "writing header" << endl;
    iterateNamespace(mRootNamespace);
}


void CppGenerator::writeGlue()
{

}


void CppGenerator::iterateNamespace(NamespacePtr namespacePtr)
{
    cout << "namespace " << namespacePtr->longName() << endl;

    for (auto memberPair : namespacePtr->members())
    {
        if (const NamespacePtr &namespacePtr = dynamic_pointer_cast<Namespace>(memberPair.second))
        {
            iterateNamespace(namespacePtr);
        }
        else if (const ClassPtr &classPtr = dynamic_pointer_cast<Class>(memberPair.second))
        {
            cout << "* class " << classPtr->longName() << endl;
            for (auto operationPair : classPtr->operations())
            {
                cout << "* operation " << operationPair.first << endl;

                for (auto paramPair : operationPair.second->params())
                {
                    cout << "* param " << paramPair.first << endl;

                    cout << mFormatter->param(paramPair.second) << endl;
                }
            }
        }
    }
}
