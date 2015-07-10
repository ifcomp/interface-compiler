#include "Components/Cpp/RpcBinaryFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void RpcBinaryFormatter::_includes(std::ostream& stream) const
{
    stream << "#if !defined(RPC_BINARY_HEADER)" << endl
           << "#define RPC_BINARY_HEADER" << endl
           << endl;

    FormatterBase::_includes(stream);

    stream << "#include \"common/rpc/binary/MessageType.hpp\"" << endl
           << "#include \"common/rpc/binary/TypeEncoding.hpp\"" << endl
           << "#include \"common/rpc/binary/OperationWrapper.hpp\"" << endl
           << "#include \"common/rpc/binary/EventWrapper.hpp\"" << endl
           << endl;
}

void RpcBinaryFormatter::_footer(std::ostream& stream, Model::RootRef root) const
{
    stream << "#endif" << endl;

    stream << endl;
    filter(stream).push<reset>() << "#if defined(RPC_BINARY_IMPLEMENTATION)" << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::binary::OperationWrapper>> everbase::internal::common::rpc::binary::OperationWrapper::operations {" << endl
           << definition(root->getNamespace(), 0)
           << "};" << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::binary::EventWrapper>> everbase::internal::common::rpc::binary::EventWrapper::events {" << endl
           << definition(root->getNamespace(), 1)
           << "};" << endl;
    filter(stream).push<reset>() << "#endif" << endl;
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::ElementRef element, std::uint8_t pass) const
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
            for( auto operation : class_->operations() )
            {
                stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::binary::OperationWrapper>>{\"" << qcname(operation) << "\", std::shared_ptr<everbase::internal::common::rpc::binary::OperationWrapper>(new "
                       << "everbase::internal::common::rpc::binary::" << qcname(operation, "_") << "())}," << endl;
            }
        }
    }
    else
    if(pass == 1)
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
                stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::binary::EventWrapper>>{\"" << qcname(event) << "\", std::shared_ptr<everbase::internal::common::rpc::binary::EventWrapper>(new "
                       << "everbase::internal::common::rpc::binary::" << qcname(event, "_") << "())}," << endl;
            }
        }
    }
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc()) << endl;
    }

    stream << "// struct " << qname(struct_) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace binary {" << endl << endl;

    stream
        << "template<>" << endl
        << "struct TypeEncoding<" << qname(struct_) << ">" << endl
        << "{" << endl
        << "    static inline void encode(everbase::internal::common::rpc::ObjectDirectory& directory, std::ostream& stream, " << qname(struct_) << " struct_)" << endl
        << "    {" << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        TypeEncoding<" << type(field->type()) << ">::encode(directory, stream, std::move(struct_." << name(field) << "));" << endl;
    }

    stream
        << "    }" << endl
        << endl;

    stream
        << "    static inline " << qname(struct_) << " decode(everbase::internal::common::rpc::ObjectDirectory& directory, std::istream& stream)" << endl
        << "    {" << endl
        << "        " << "return " << qname(struct_) << "(" << endl;

    for( auto field : indices(struct_->fields()) )
    {
        stream
            << "            TypeEncoding<" << type(field.value()->type()) << ">::decode(directory, stream)"<< (field.last() ? "" : ",") << endl;
    }

    stream
        << "        );" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::binary" << endl << endl;

    stream << "// struct " << qname(struct_) << ": }" << endl << endl;
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    for( auto operation : class_->operations() )
    {
        stream << definition(operation) << endl;
    }

    for( auto event : class_->events() )
    {
        stream << definition(event) << endl;
    }
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(event->parent());

    if(!class_)
        throw std::runtime_error("invalid event");

    stream << "// event " << qname(event) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace binary {" << endl << endl;

    stream
        << "struct " << qcname(event, "_") << " : public EventWrapper" << endl
        << "{" << endl;


    // *** encode

    stream
        << "    virtual inline void encode(everbase::internal::common::rpc::ObjectDirectory& directory, std::ostream& stream, everbase::common::Event& event) const override" << endl
        << "    {" << endl
        << "        " << qname(event) << "& event_ = dynamic_cast<" << qname(event) << "&>(event);" << endl;

    for( auto value : event->values() )
    {
        stream
            << "        TypeEncoding<" << type(value->type()) << ">::encode(directory, stream, event_." << name(value) << ");" << endl;
    }

    stream
        << "    }" << endl
        << endl;


    // *** decode

    stream
        << "    virtual inline std::shared_ptr<everbase::common::Event> decode(everbase::internal::common::rpc::ObjectDirectory& directory, std::istream& stream) const override" << endl
        << "    {" << endl
        << "        std::shared_ptr<" << qname(event) << "> event = std::make_shared<" << qname(event) << ">();" << endl;

    for( auto value : event->values() )
    {
        stream
            << "        event->" << name(value) << " = TypeEncoding<" << type(value->type()) << ">::decode(directory, stream);" << endl;
    }

    stream
        << "        return event;" << endl
        << "    }" << endl
        << endl;

    stream << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::binary" << endl << endl;

    stream << "// event " << qname(event) << ": }" << endl << endl;
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("invalid operation");

    if ( operation->doc() )
    {
        stream << doc(operation->doc()) << endl;
    }

    stream << "// operation " << qname(operation) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace binary {" << endl << endl;

    stream << "class " << qcname(operation, "_") << " : public OperationWrapper" << endl
           << "{" << endl;

    stream << "    virtual inline void execute(common::rpc::ObjectDirectory& directory, std::istream& call, std::ostream& response) const override" << endl
           << "    {" << endl;

    if( !operation->isStatic() )
    {
        stream << "        std::shared_ptr<" << qname(class_) << "> this_ = TypeEncoding<std::shared_ptr<" << qname(class_) << ">>::decode(directory, call);" << endl << endl;
    }

    for( auto param : operation->params() )
    {
        stream << "        " << type(param->type()) << " param_" << name(param) << " = TypeEncoding<" << type(param->type()) << ">::decode(directory, call);" << endl;
    }

    stream << endl;

    if( operation->result() )
    {
        stream << "        " << type(operation->result()->type()) << " result;" << endl;
    }

    stream << endl;

    stream << "        bool hasException = false;" << endl
           << "        std::string exception;" << endl << endl;

    stream << "        try" << endl
           << "        {" << endl;

    if( operation->result() )
    {
        stream << "            result = ";
    }
    else
    {
        stream << "            ";
    }

    if( !operation->isStatic() )
    {
        stream << "this_->" << name(operation);
    }
    else
    {
        stream << qname(operation);
    }

    stream << "(";

    for( auto param : indices(operation->params()) )
    {
        stream << "std::move(param_" << name(param.value()) << (!param.last() ? "), " : ")");
    }

    stream << ");" << endl;

    stream << "        }" << endl
           << "        catch(const std::exception& e)" << endl
           << "        {" << endl
           << "            hasException = true;" << endl
           << "            exception = e.what();" << endl
           << "        }" << endl
           << "        catch(...)" << endl
           << "        {" << endl
           << "            hasException = true;" << endl
           << "            exception = \"unknown exception\";" << endl
           << "        }" << endl
           << endl;

    stream << "        if(hasException)" << endl
           << "        {" << endl
           << "            TypeEncoding<bool>::encode(directory, response, true);" << endl
           << "            TypeEncoding<std::string>::encode(directory, response, std::move(exception));" << endl
           << "        }" << endl
           << "        else" << endl
           << "        {" << endl
           << "            TypeEncoding<bool>::encode(directory, response, false);" << endl;

    if( operation->result() )
    {
        stream << "            TypeEncoding<" << type(operation->result()->type()) << ">::encode(directory, response, std::move(result));" << endl;
    }

    stream << "        }" << endl
           << endl;

    stream << "    }" << endl;

    stream << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::binary" << endl << endl;

    stream << "// operation " << qname(operation) << ": }" << endl << endl;

}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
