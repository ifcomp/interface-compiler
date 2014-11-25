#include "Components/Ruby/EnumsFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void EnumsFormatter::_includes(std::ostream& stream) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

void EnumsFormatter::_definition(std::ostream& stream, Model::StructRef enum_) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void EnumsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(enum_->parent());

    if(!namespace_)
        throw std::runtime_error("invalid enum");

    if ( enum_->doc() )
    {
        stream << doc(enum_->doc()) << endl;
    }

    stream << "// enum " << qname(enum_) << ": {" << endl << endl;

    stream << "EVERBASE_RUBY_ENUM(" << qcname(enum_, "_") << ", " << qcname(namespace_, "_") << ", \"" << name(enum_) << "\")" << endl << endl;

    for( auto value : enum_->values() )
    {
        stream << "EVERBASE_RUBY_ENUM_VALUE(" << qcname(value, "_") << ", " << qcname(enum_, "_") << ", \"" << name(value) << "\", 0x" << std::hex << static_cast<std::uint64_t>(value->value()) << ")" << endl;
    }

    stream << endl;

    stream << "template<>" << endl
           << "struct TypeEncoding<" << cpp.qname(enum_) << ">" << endl
           << "{" << endl
           << "    using unencoded_type = " << cpp.qname(enum_) << ";" << endl
           << "    using encoded_type = VALUE;" << endl
           << endl
           << "    inline static unencoded_type decode(encoded_type src)" << endl
           << "    {" << endl
           << "        return static_cast<unencoded_type>(NUM2INT(src));" << endl
           << "    }" << endl
           << endl
           << "    inline static encoded_type encode(unencoded_type src)" << endl
           << "    {" << endl
           << "        return INT2NUM(static_cast<int>(src));" << endl
           << "    }" << endl
           << "};" << endl << endl;

    stream << "// enum " << qname(enum_) << ": }" << endl << endl;
}

void EnumsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
