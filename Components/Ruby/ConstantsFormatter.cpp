#include "Components/Ruby/ConstantsFormatter.hpp"

#include <sstream>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void ConstantsFormatter::_includes(std::ostream& stream) const
{
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

void ConstantsFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    for( auto constant : class_->constants() )
    {
        stream << definition(constant) << endl;
    }
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    std::stringstream ss;
    ss << name(constant);
    std::string str = ss.str();
    boost::to_upper(str);

    stream << str << " = ";

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

            case Primitive::Underlying::STRING:
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
            {
                auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                stream << "\"" << uuid << "\"";
            }
            break;
            
            default:
                throw std::runtime_error("not supported");
        }
    }
    
    stream << endl;
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void ConstantsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
