#include "Components/Cpp/HeaderFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void HeaderFormatter::_includes(std::ostream& stream) const
{
    stream
        << "#pragma once" << endl
        << endl;

    FormatterBase::_includes(stream);

    stream
        << "#include \"everbase/common/EnableSharedFromThisMultiple.hpp\"" << endl
        << endl;
}

void HeaderFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
    if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        stream << "namespace " << name(namespace_) << endl << "{" << endl;

        for ( auto element : namespace_->elements() )
        {
            filter(stream).push<indent>(config.indentData) << forwards(element);
        }

        stream << "}" << endl << endl;
    }
    else
    if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        stream << "class " << name(class_) << ";" << endl;
        stream << "using " << name(class_) << "Ref = std::shared_ptr<" << name(class_) << ">;" << endl << endl;
    }
    else
    if( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(element) )
    {
        stream << "struct " << name(struct_) << ";" << endl << endl;
    }
}

void HeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }

    stream << "struct " << name(struct_) << endl << "{" << endl;

    for (auto field : struct_->fields())
    {
        if ( field->doc() )
        {
            filter(stream).push<indent>(config.indentData) << doc(field->doc());
        }

        filter(stream).push<indent>(config.indentData) << param(field) << ";" << endl << endl;
    }

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

    stream << "class " << name(class_) << " : public virtual ";

    if(class_->super())
    {
        if( auto super = std::dynamic_pointer_cast<Model::Class>(std::dynamic_pointer_cast<Type>(class_->super())->primary()) )
        {
            stream << qname(super);
        }
        else
            throw std::runtime_error("invalid super type");
    }
    else
    {
        stream << "everbase::common::EnableSharedFromThisMultiple<" << name(class_) << ">";
    }

    stream << endl << "{" << endl;

    if(class_->constants().size() > 0)
    {
        stream << "public:" << endl;

        for ( auto constant : class_->constants() )
        {
            filter(stream).push<indent>(config.indentData) << definition(constant) << endl;
        }
    }

    if(class_->operations().size() > 0)
    {
        stream << "public:" << endl;

        for( auto operation : class_->operations() )
        {
            filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
        }
    }

    if(class_->events().size() > 0)
    {
        stream << "public:" << endl;

        for ( auto event : class_->events() )
        {
            filter(stream).push<indent>(config.indentData) << definition(event) << endl;
        }
    }

    stream << "public:" << endl;

    filter(stream).push<indent>(config.indentData)
        << name(class_) << "(const " << name(class_) << "& other) = delete;" << endl
        << name(class_) << "(" << name(class_) << "&& other) = delete;" << endl
        << "inline virtual ~" << name(class_) << "() { }" << endl << endl;

    filter(stream).push<indent>(config.indentData)
        << name(class_) << "& operator=(const " << name(class_) << "& other) = delete;" << endl
        << name(class_) << "& operator=(" << name(class_) << "&& other) = delete;" << endl << endl;

    stream << "protected:" << endl;

    filter(stream).push<indent>(config.indentData)
        << "inline " << name(class_) << "() { }" << endl;

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "EVERBASE_LIB_EXPORT static const " << type(constant->type()) << " " << name(constant) << "; // = ";

    if( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
                stream << "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
                stream << (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "{ { ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " } }";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

    stream << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "struct " << name(event) << " : public everbase::common::Event" << endl << "{" << endl;

    filter(stream).push<indent>(config.indentData)
        << "EVERBASE_LIB_EXPORT static const char TYPE_NAME[]; // = \"" << qcname(event) << "\"" << endl
        << "EVERBASE_LIB_EXPORT static const bool TYPE_EXTERNAL; // = true" << endl
        << endl;

    for (auto value : event->values())
    {
        if ( value->doc() )
        {
            filter(stream).push<indent>(config.indentData) << doc(value->doc());
        }

        filter(stream).push<indent>(config.indentData) << param(value) << ";" << endl << endl;
    }

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << ( operation->isStatic() ? "EVERBASE_LIB_EXPORT " : "" ) << signature(operation) << (!operation->isStatic() ? " = 0" : "") << ";" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }

    stream << "enum class " << name(enum_) << endl << "{" << endl;

    for (auto value : indices(enum_->values()))
    {
        filter(stream).push<indent>(config.indentData) << definition(value.value()) << (!value.last() ? "," : "") << endl << endl;
    }

    stream << "};" << endl << endl;

    if( enum_->isBitfield() )
    {
        std::string bitops( "|&^" );
        for ( char &op :bitops )
        {
            stream << "inline " << name(enum_) << " operator" << op << "( " << name(enum_) << " a, " << name(enum_) << " b )" << endl
                   << "{" << endl;
            filter(stream).push<indent>(config.indentData) << "return static_cast<" << name(enum_) << ">(static_cast<int>(a) " << op << " static_cast<int>(b));" << endl;
            stream << "}" << endl << endl;
        }

        stream << "inline " << name(enum_) << " operator" << "~" << "( " << name(enum_) << " a )" << endl
               << "{" << endl;
        filter(stream).push<indent>(config.indentData) << "return static_cast<" << name(enum_) << ">( ~static_cast<int>(a) );" << endl;
        stream << "}" << endl << endl;

        std::vector<std::string> assops( { "|=", "&=", "^=" } );
        for ( auto &op :assops )
        {
            stream << "inline " << name(enum_) << "& operator" << op << "( " << name(enum_) << "& a, " << name(enum_) << " b )" << endl
                   << "{" << endl;
            filter(stream).push<indent>(config.indentData) << "a = static_cast<" << name(enum_) << ">( static_cast<int>(a) " << op[0] << " static_cast<int>(b) );" << endl;
            filter(stream).push<indent>(config.indentData) << "return a;" << endl;
            stream << "}" << endl << endl;
        }
    }

    stream << "inline std::string to_string( " << name(enum_) << " enumerator )" << endl << "{" << endl;
    if( enum_->isBitfield() )
    {
        filter(stream).push<indent>(config.indentData) << "std::string values( \"\" );" << endl << endl;

    }
    else
    {
        filter(stream).push<indent>(config.indentData) << "switch( enumerator )" << endl << "{" << endl;
    }

    for (auto value : enum_->values())
    {

        if( enum_->isBitfield() )
        {
            filter(stream).push<indent>(config.indentData) << "if( ( enumerator & " << name(enum_) << "::" << name(value) << " ) == " << name(enum_) << "::" << name(value) << " )" << endl
                                                           << "{" << endl
                                                           << "    values += ( values.empty() ? \"\" : \" | \" );" << endl
                                                           << "    values += \"" << name(value) << "\";" << endl
                                                           << "}" << endl << endl;
        }
        else
        {
            filter(stream).push<indent>(config.indentData) << "    case " << name(enum_) << "::" << name(value) << ":" << endl
                                                           << "        return \"" << name(value) << "\";" << endl;
        }
    }

    if( enum_->isBitfield() )
    {
        filter(stream).push<indent>(config.indentData) << "return values;" << endl;
    }
    else
    {
        filter(stream).push<indent>(config.indentData) << "default:" << endl << "    return std::string( \"UNKNOWN(\" ) + std::to_string( (int)enumerator ) + \")\";" << endl << "}" << endl;
    }

    stream << "}" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    if ( value->doc() )
    {
        stream << doc(value->doc());
    }

    stream << name(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value());
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
