#include "Components/Cpp/RpcBinaryFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void RpcBinaryFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include \"common/rpc/binary/MessageType.hpp\"" << endl
           << "#include \"common/rpc/binary/TypeEncoding.hpp\"" << endl
           << endl;
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
        << "    static inline void encode(everbase::internal::common::rpc::ObjectDirectory& directory, std::ostream& stream, " << qname(struct_) << " value)" << endl
        << "    {" << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        TypeEncoding<" << type(field->type()) << ">::encode(directory, stream, std::move(source." << name(field) << "));" << endl;
    }

    stream
        << "    }" << endl
        << endl;

    stream
        << "    static inline " << qname(struct_) << " decode(everbase::internal::common::rpc::ObjectDirectory& directory, std::istream& stream)" << endl
        << "    {" << endl
        << "        " << qname(struct_) << " struct_;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        struct_." << name(field) << " = TypeEncoding<" << type(field->type()) << ">::decode(directory, stream);" << endl;
    }

    stream
        << endl
        << "        return struct_;" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::binary" << endl << endl;

    stream << "// struct " << qname(struct_) << ": }" << endl << endl;
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
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
        << "struct TypeEncoding<" << qname(event) << ">" << endl
        << "{" << endl;


    // *** encode

    stream
        << "    static inline void encode(everbase::internal::common::rpc::ObjectDirectory& directory, std::ostream& stream, " << qname(event) << " event)" << endl
        << "    {" << endl;

    for( auto value : event->values() )
    {
        stream
            << "        TypeEncoding<" << type(value->type()) << ">::encode(directory, stream, event." << name(value) << ");" << endl;
    }

    stream
        << "    }" << endl
        << endl;


    // *** decode

    stream
        << "    static inline std::vector<boost::any> decode(everbase::internal::common::rpc::ObjectDirectory& directory, std::istream& stream)" << endl
        << "    {" << endl
        << "        " << qname(event) << " event;" << endl;

    for( auto value : event->values() )
    {
        stream
            << "        event." << name(value) << " = TypeEncoding<" << type(value->type()) << ">::decode(directory, stream);" << endl;
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
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void RpcBinaryFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
