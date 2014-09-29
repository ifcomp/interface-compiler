#include "Components/StandardParser.hpp"
#include "Components/NamespaceReader.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components {

Model::RootRef StandardParser::execute ( const ConfigProvider& cprov, std::istream& input ) const
{
	using namespace std;

    Model::NamespaceRef rootNamespace = std::make_shared<Model::Namespace>("::");
    NamespaceReader reader(rootNamespace);

    reader.parseFile(input);
    reader.resolveTypesInNamespace(rootNamespace);
    reader.listKnownTypes();

    Model::RootRef root = std::make_shared<Model::Root>();
    root->setNamespace(rootNamespace);

    return root;
}

} } } // namespace: Everbase::InterfaceCompiler::Components
