#include "Components/Cpp/JsonEncodingFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void JsonEncodingFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include <json_spirit/json_spirit.h>" << endl
           << "#include \"Everbase/JSON/TypeEncoding.hpp\"" << endl
           << endl;
}

void JsonEncodingFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc()) << endl;
    }

    stream << "// struct " << qname(struct_) << ": {" << endl << endl;

    stream << "namespace Everbase { namespace JSON {" << endl << endl;

    stream
        << "template<typename DirectoryT>" << endl
        << "struct TypeEncoding<DirectoryT, " << qname(struct_) << ">" << endl
        << "{" << endl
        << "    static json_spirit::mValue encode(DirectoryT& directory, " << qname(struct_) << " source)" << endl
        << "    {" << endl
        << "        json_spirit::mObject object;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        object[\"" << cname(field) << "\"] = TypeEncoding<DirectoryT, " << type(field->type()) << ">::encode(directory, source." << name(field) << ");" << endl;
    }

    stream
        << endl
        << "        return json_spirit::mValue(object);" << endl
        << "    }" << endl
        << endl;

    stream
        << "    static " << qname(struct_) << " decode(DirectoryT& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        const json_spirit::mObject& object = source.get_obj();" << endl
        << "        " << qname(struct_) << " struct_;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        struct_." << name(field) << " = TypeEncoding<DirectoryT, " << type(field->type()) << ">::decode(directory, object.at(\"" << cname(field) << "\"));" << endl;
    }

    stream
        << endl
        << "        return struct_;" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } // namespace: Everbase::JSON" << endl << endl;

    stream << "// struct " << name(struct_) << ": }" << endl << endl;
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// class " << qname(class_) << ": {" << endl << endl;

    stream << "namespace Everbase { namespace JSON {" << endl << endl;

    stream
        << "template<typename DirectoryT>" << endl
        << "struct TypeEncoding<DirectoryT, " << qname(class_) << "Ref>" << endl
        << "{" << endl
        << "    static json_spirit::mValue encode(DirectoryT& directory, " << qname(class_) << "Ref source)" << endl
        << "    {" << endl
        << "        return json_spirit::mValue(directory.template registerObject<" << qname(class_) << ">(source));" << endl
        << "    }" << endl
        << endl
        << "    static " << qname(class_) << "Ref decode(DirectoryT& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        return directory.template lookupObject<" << qname(class_) << ">(source.get_uint64());" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } // namespace: Everbase::JSON" << endl << endl;

    stream << "// class " << name(class_) << ": }" << endl << endl;
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc()) << endl;
    }

    stream << "// enum " << qname(enum_) << ": {" << endl << endl;

    stream << "namespace Everbase { namespace JSON {" << endl << endl;

    stream
        << "template<typename DirectoryT>" << endl
        << "struct TypeEncoding<DirectoryT, " << qname(enum_) << ">" << endl
        << "{" << endl
        << "    static json_spirit::mValue encode(DirectoryT& directory, " << qname(enum_) << " source)" << endl
        << "    {" << endl
        << "        return json_spirit::mValue(static_cast<std::uint64_t>(source));" << endl
        << "    }" << endl
        << endl
        << "    static " << qname(enum_) << " decode(DirectoryT& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        return static_cast<" << qname(enum_) << ">(source.get_uint64());" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } // namespace: Everbase::JSON" << endl << endl;

    stream << "// enum " << name(enum_) << ": }" << endl << endl;
}

void JsonEncodingFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
