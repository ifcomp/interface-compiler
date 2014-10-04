#include "Components/StandardParser.hpp"
#include "Components/NamespaceReader.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components {

Model::RootRef StandardParser::execute ( const ConfigProvider& cprov, std::istream& input ) const
{
	using namespace std;

    auto tmpNamespace = std::make_shared<Model::Namespace>("Root");
    NamespaceReader reader(tmpNamespace);

    reader.parseFile(input);
    reader.resolveTypesInNamespace(tmpNamespace);
    reader.listKnownTypes();

    auto rootNamespace = std::dynamic_pointer_cast<Model::Namespace>(tmpNamespace->members()[0]);
    rootNamespace->setParentObject(nullptr);

    Model::RootRef root = std::make_shared<Model::Root>();
    root->setNamespace(rootNamespace);

    return root;
}

} } } // namespace: Everbase::InterfaceCompiler::Components
