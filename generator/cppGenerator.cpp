#include "generator/cppGenerator.hpp"
#include "generator/cppFormatter.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace std;

#define WS  mFormatter->indent()       ///< shortcut for inserting indentation


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
    for (auto memberPair : namespacePtr->members())
    {
        if (const NamespacePtr &namespacePtr = dynamic_pointer_cast<Namespace>(memberPair.second))
        {
            cout << WS << "namespace " << mFormatter->name(namespacePtr) << endl;
            cout << WS << "{" << endl;
            mFormatter->beginIndent(namespacePtr);
            iterateNamespace(namespacePtr);
            mFormatter->endIndent();
            cout << WS << "} // namespace " << mFormatter->name(namespacePtr) << endl;
        }
        else if (const ClassPtr &classPtr = dynamic_pointer_cast<Class>(memberPair.second))
        {
            cout << WS << "class " << mFormatter->name(classPtr) << endl;
            cout << WS << "{" << endl;
            mFormatter->beginIndent(classPtr);

            for (auto operationPair : classPtr->operations())
            {
                cout << mFormatter->operation(operationPair.second) << endl;
            }

            mFormatter->endIndent();
            cout << WS << "}" << endl << endl;
        }
    }
}
