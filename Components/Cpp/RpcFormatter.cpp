#include "Components/Cpp/RpcFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void RpcFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include <boost/any.hpp>" << endl
           << "#include \"common/rpc/OperationWrapper.hpp\"" << endl
           << "#include \"common/rpc/EventEncoding.hpp\"" << endl
           << endl;
}

void RpcFormatter::_footer(std::ostream& stream, Model::RootRef root) const
{
    stream << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::OperationWrapper>> everbase::internal::common::rpc::OperationWrapper::operations {" << endl
           << backwards(root->getNamespace())
           << "};" << endl;

    stream << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::EventEncoding>> everbase::internal::common::rpc::EventEncoding::events {" << endl
           << definition(root->getNamespace(), 0)
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
            stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::OperationWrapper>>{\"" << qcname(operation) << "\", std::shared_ptr<everbase::internal::common::rpc::OperationWrapper>(new "
                   << "everbase::internal::common::rpc::" << qcname(operation, "_") << "())}," << endl;
        }
    }
}

void RpcFormatter::_definition(std::ostream& stream, Model::ElementRef element, std::uint8_t pass) const
{
    if(pass == 0)
    {
        if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
        {
            for ( auto element : namespace_->elements() )
            {
                filter(stream).push<indent>(config.indentData) << definition(element, pass);
            }
        }
        else
        if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
        {
            for( auto event : class_->events() )
            {
                stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::EventEncoding>>{\"" << qcname(event) << "\", std::shared_ptr<everbase::internal::common::rpc::EventEncoding>(new "
                       << "everbase::internal::common::rpc::" << qcname(event, "_") << "())}," << endl;
            }
        }
    }
}

void RpcFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc()) << endl;
    }
    
    stream << "// namespace " << qname(namespace_) << ": {" << endl << endl;

    for ( auto element : namespace_->elements() )
    {
        stream << definition(element);
    }

    stream << "// namespace " << qname(namespace_) << ": }" << endl << endl;
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

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc {" << endl << endl;

    for( auto operation : class_->operations() )
    {
        filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
    }

    for( auto event : class_->events() )
    {
        filter(stream).push<indent>(config.indentData) << definition(event) << endl;
    }

    stream << "} } } } // namespace: everbase::internal::common::rpc" << endl << endl;

    stream << "// class " << name(class_) << ": }" << endl << endl;
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(event->parent());

    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream
        << "struct " << qcname(event, "_") << " : public everbase::internal::common::rpc::EventEncoding" << endl
        << "{" << endl
        << "    virtual std::vector<boost::any> encode(const everbase::common::Event& event) const" << endl
        << "    {" << endl;

    if ( !event->values().empty() )
    {
        stream
        << "        const " << qname(event) << "& decoded = dynamic_cast<const " << qname(event) << "&>(event);" << endl;
    }
    
        stream
        << "        std::vector<boost::any> encoded;" << endl;

    std::size_t i = 0;

    for( auto value : event->values() )
    {
        stream << "        encoded.push_back(decoded." << name(value) << ");" << endl;
        i += 1;
    }

    stream
        << "        return encoded;" << endl
        << "    }" << endl
        << endl
        << "    virtual std::shared_ptr<everbase::common::Event> decode(std::vector<boost::any> event) const" << endl
        << "    {" << endl
        << "        std::shared_ptr<" << qname(event) << "> decoded(new " << qname(event) << ");" << endl;

    i = 0;

    for( auto value : event->values() )
    {
        stream << "        decoded->" << name(value) << " = boost::any_cast<" << type(value->type()) << ">(event[" << i << "]);" << endl;
        i += 1;
    }

    stream
        << "        return decoded;" << endl
        << "    }" << endl
        << "};" << endl;
}

void RpcFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if ( operation->doc() )
    {
        stream << doc(operation->doc());
    }

    stream
        << "struct " << qcname(operation, "_") << " : public everbase::internal::common::rpc::OperationWrapper" << endl
        << "{" << endl
        << "    inline virtual boost::any call(std::vector<boost::any> params) const" << endl
        << "    {" << endl;

    if(operation->isStatic())
    {
        stream
            << "        " << (operation->result() ? "return " : "") << qname(class_) << "::" << name(operation) << "(";
    }
    else
    {
        stream
            << "        " << (operation->result() ? "return " : "") << "boost::any_cast<std::shared_ptr<" << qname(class_) << ">>(params[0])->" << name(operation) << "(";
    }

    std::size_t i = operation->isStatic() ? 0 : 1;

    for( auto param : indices(operation->params()) )
    {
        stream << "boost::any_cast<" << type(param.value()->type()) << ">(params[" << i << "])" << (!param.last() ? ", " : "");
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
