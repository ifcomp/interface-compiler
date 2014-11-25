#include "Components/Ruby/StructsFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void StructsFormatter::_includes(std::ostream& stream) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    Model::NamespaceRef parent;

    if(namespace_->parent())
    {
        parent = std::dynamic_pointer_cast<Model::Namespace>(namespace_->parent());
    }

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

void StructsFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(struct_->parent());

    if(!namespace_)
        throw std::runtime_error("invalid struct");

    if ( struct_->doc() )
    {
        stream << doc(struct_->doc()) << endl;
    }

    stream << "// struct " << qname(struct_) << ": {" << endl << endl;

    stream << "EVERBASE_RUBY_STRUCT(" << qcname(struct_, "_") << ", " << qcname(namespace_, "_") << ", \"" << name(struct_) << "\")" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream << "EVERBASE_RUBY_STRUCT_FIELD(" << qcname(field, "_") << ", " << qcname(struct_, "_") << ", \"" << name(field) << "\")" << endl;
    }

    stream << endl;

    stream
        << "template<>" << endl
        << "struct TypeEncoding<" << cpp.qname(struct_) << ", void>" << endl
        << "{" << endl
        << "    using unencoded_type = " << cpp.qname(struct_) << ";" << endl
        << "    using encoded_type = VALUE;" << endl
        << endl
        << "    static inline encoded_type encode(unencoded_type src)" << endl
        << "    {" << endl
        << "        encoded_type struct_;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        rb_iv_set(struct_, \"@" << name(field) << "\", TypeEncoding<" << cpp.type(field->type()) << ">::encode(src." << cpp.name(field) << "));" << endl;
    }

    stream
        << "    }" << endl
        << endl;

    stream
        << "    static inline unencoded_type decode(encoded_type src)" << endl
        << "    {" << endl
        << "        unencoded_type struct_;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        struct_." << cpp.name(field) << " = TypeEncoding<" << cpp.type(field->type()) << ">::decode(rb_iv_get(src, \"@" << name(field) << "\"));" << endl;
    }

    stream
        << endl
        << "        return struct_;" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "// struct " << qname(struct_) << ": }" << endl << endl;
}

void StructsFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void StructsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
