#include "formatter/cpp/cppHeaders.hpp"
#include "formatter/cpp/cppFormatter.hpp"
#include "formatter/stream/basicIndent.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace Api::Formatter::Stream;
using namespace std;


CppHeaders::CppHeaders(Api::Model::NamespacePtr rootNamespace, std::string outputDirectory)
    : Generator(rootNamespace, outputDirectory)
{
    mFormatter = make_shared<CppFormatter>("../api-generator/yaml/cpp/config.yaml");
}

void CppHeaders::generate()
{
    cout << "writing header" << endl;
    iterateNamespace(mRootNamespace);
}


void CppHeaders::iterateNamespace(NamespacePtr namespacePtr)
{
    for (auto memberPair : namespacePtr->members())
    {
        if (const NamespacePtr &namespacePtr = dynamic_pointer_cast<Namespace>(memberPair.second))
        {
            cout << "namespace ";
            mFormatter->name(cout, namespacePtr);
            cout << endl;

            cout << "{" << endl;
            {
                indent i(cout);
                iterateNamespace(namespacePtr);
            }

            cout << "} // namespace ";
            mFormatter->name(cout, namespacePtr);
            cout << endl << endl;
        }
        else if (const ClassPtr &classPtr = dynamic_pointer_cast<Class>(memberPair.second))
        {
            cout << "class ";
            mFormatter->name(cout, classPtr);
            cout << endl;

            cout << "{" << endl;
            {
                indent i(cout);
                for (auto operationPair : classPtr->operations())
                {
                    mFormatter->operation(cout, operationPair.second);
                    cout << ";" << endl;
                }
            }

            cout << "}" << endl << endl;
        }
    }
}
