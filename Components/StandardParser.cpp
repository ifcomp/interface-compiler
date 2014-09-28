#include "Components/StandardParser.hpp"
#include "Components/NamespaceReader.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components {

Model::RootRef StandardParser::execute ( const ConfigProvider& cprov, std::istream& input ) const
{
    Model::NamespaceRef ns = std::make_shared<Model::Namespace>("::");
    NamespaceReader read(ns);
    read.parseFile(input);

    Model::RootRef root = std::make_shared<Model::Root>();
    root->setNamespace(ns);

    return root;
}

} } } // namespace: Everbase::InterfaceCompiler::Components
