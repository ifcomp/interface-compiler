#include "Components/Cpp/RpcFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void RpcFormatter::_footer(std::ostream& stream, Model::RootRef root) const
{
    stream << endl;
    stream << "const std::map<std::string, std::unique_ptr<OperationBase>> Everbase::Rpc::OperationMap::operations = {" << endl
           << backwards(root->getNamespace())
           << "};" << endl;
}

void RpcFormatter::_backwards(std::ostream& stream, Model::ElementRef element) const
{
    if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        for ( auto element : namespace_->elements() )
        {
            filter(stream).push<indent>(config.indentData) << backwards(element);
        }
    }
    else
    if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        for( auto operation : class_->operations() )
        {
            stream << "std::pair<std::string, std::unique_ptr<OperationBase>>{\"" << qcname(operation) << "\", std::unique_ptr<OperationBase>(new "
                   << qname(class_) << "Rpc::"
                   << name(operation->longName(), operation->shortName(), config.nameConfig<Model::Class>()) << "())}" << endl;
        }
    }
}

void RpcFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void RpcFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// class " << name(class_) << ": {" << endl << endl;

    stream << "namespace " << name(class_->longName() + "Rpc", class_->shortName() + "Rpc", config.nameConfig<Model::Namespace>()) << endl
           << "{" << endl;

    for( auto operation : class_->operations() )
    {
        filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
    }

    stream << "}" << endl << endl;

    stream << "// class " << name(class_) << ": }" << endl << endl;
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if ( operation->doc() )
    {
        stream << doc(operation->doc());
    }

    stream
        << "struct " << name(operation->longName(), operation->shortName(), config.nameConfig<Model::Class>()) << " : public Everbase::Rpc::Operation" << endl
        << "{" << endl
        << "    inline virtual boost::any call(std::vector<boost::any> params)" << endl
        << "    {" << endl;

    if(operation->isStatic())
    {
        stream
            << "        " << (operation->result() ? "return " : "") << qname(class_) << "::" << name(operation) << "(";
    }
    else
    {
        stream
            << "        " << (operation->result() ? "return " : "") << "std::any_cast<std::shared_ptr<" << qname(class_) << ">>(param[0])->" << name(operation) << "(";
    }

    std::size_t i = operation->isStatic() ? 0 : 1;

    for( auto param : indices(operation->params()) )
    {
        stream << "std::any_cast<" << type(param.value()->type()) << ">(param[" << i << "])" << (!param.last() ? ", " : "");
        i += 1;
    }

    stream
        << ");" << endl;

    if(!operation->result())
    {
        stream
            << "        return boost::any();" << endl;
    }

    stream
        << "    }" << endl
        << "};" << endl;
}

void RpcFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void RpcFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
