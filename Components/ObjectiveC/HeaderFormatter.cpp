#include "Components/ObjectiveC/HeaderFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace ObjectiveC {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void HeaderFormatter::_includes(std::ostream& stream) const
{
    stream
        << "#pragma once" << endl
        << endl
        << "#ifdef __cplusplus" << endl
        << "extern \"C\" {" << endl
        << "#endif" << endl;

    FormatterBase::_includes(stream);
}

void HeaderFormatter::_footer(std::ostream& stream, Model::RootRef root) const
{
    stream << "#ifdef __cplusplus" << endl
           << "} // extern \"C\"" << endl
           << "#endif" << endl;
}

void HeaderFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
    if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        stream << endl << "// namespace " << qcname(namespace_, ".") << ": begin" << endl;

        for ( auto element : namespace_->elements() )
        {
            stream << forwards(element);
        }

        stream << "// namespace " << qcname(namespace_, ".") << ": end" << endl << endl;
    }
    else
    if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        stream << "@class " << qname(class_) << ";" << endl;
        for ( auto event : class_->events() )
        {
            stream << "@class " << qname(class_) << name(event) << ";" << endl;
        }
    }
    else
    if( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(element) )
    {
        stream << "@class " << qname(struct_) << ";" << endl;
    }
}

void HeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }

    stream << "@interface " << qname(struct_) << " : NSObject" << endl << endl;

    for (auto field : struct_->fields())
    {
        if ( field->doc() )
        {
            stream << doc(field->doc());
        }

        stream << "@property(assign) " << type(field->type()) << " " << name(field) << ";" << endl << endl;
    }

    stream << "@end // interface " << qname(struct_) << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

    stream << "@interface " << qname(class_) << " : ";

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
        stream << "NSObject";
    }

    stream << " {" << endl;

    if(!class_->super())
    {
        filter(stream).push<indent>() << "@protected void* _data;" << endl;
    }

    stream << "}" << endl << endl;

    if(class_->operations().size() > 0)
    {
        stream << "// operations:" << endl;

        for( auto operation : class_->operations() )
        {
            stream << definition(operation) << endl;
        }
    }

    stream << "// allocation & deallocation:" << endl;

    stream << "- (id) init;" << endl;
    stream << "- (id) initWithData:(void*)data;" << endl;
    stream << "- (void) dealloc;" << endl;
    stream << "- (void*) data;" << endl;

    stream << "@end // interface " << qname(class_) << endl << endl;

    if(class_->constants().size() > 0)
    {
        stream << "// constants" << endl;

        for ( auto constant : class_->constants() )
        {
            stream << definition(constant) << endl;
        }
    }

    if(class_->events().size() > 0)
    {
        stream << "// events:" << endl;

        for ( auto event : class_->events() )
        {
            stream << definition(event) << endl;
        }
    }
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    auto class_ = std::dynamic_pointer_cast<Class>(constant->parent());

    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "extern const " << type(constant->type()) << " " << qname(class_) << name(constant) << "; // = ";

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
                stream << "@\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "[NSUUID initWithUUIDBytes:{ ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " }]";
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

    stream << "@interface " << qname(event) << " : EVBEvent" << "{" << endl << "}" << endl;

    for (auto value : event->values())
    {
        if ( value->doc() )
        {
            stream << doc(value->doc());
        }

        stream << "@property(assign) " << type(value->type()) << " " << name(value) << ";" << endl << endl;
    }

    stream << "+ (uint32_t) addObserver:(id)observer selector:(SEL)selector;" << endl << endl;

    stream << "@end // interface " << qname(event) << endl << endl;

    stream << "extern const NSString* " << qname(event) << "TypeName; // = @\"" << qcname(event) << "\"" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation) << ";" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }

    stream << "typedef enum {" << endl;

    for (auto value : indices(enum_->values()))
    {
        filter(stream).push<indent>(config.indentData) << definition(value.value()) << (!value.last() ? "," : "") << endl << endl;
    }

    stream << "} " << qname(enum_) << ";" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    if ( value->doc() )
    {
        stream << doc(value->doc());
    }

    stream << qname(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value());
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::ObjectiveC
