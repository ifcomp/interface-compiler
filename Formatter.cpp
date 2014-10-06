#include "Formatter.hpp"

#include <boost/regex.hpp>
#include <iostream>
#include <sstream>

namespace Everbase { namespace InterfaceCompiler {

Formatter::Formatter(FormatterConfig config)
    : config(config)
{
}

void Formatter::execute ( Model::RootRef input, std::ostream& output ) const
{
	output << definition(input);
}

FormatToken<Model::IdentifiableRef> Formatter::qname(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::_qname, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<Model::IdentifiableRef> Formatter::name(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::_name, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<std::string, std::string, FormatterConfig::NameConfigBase> Formatter::name(std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const
{
    return FormatToken<std::string, std::string, FormatterConfig::NameConfigBase> { this, &Formatter::_name, std::tuple<std::string, std::string, FormatterConfig::NameConfigBase> { longName, shortName, config } };
}

FormatToken<Model::IdentifiableRef> Formatter::qcname(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::_qcname, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<Model::IdentifiableRef> Formatter::cname(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::_cname, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<std::string> Formatter::cname(std::string longName) const
{
    return FormatToken<std::string> { this, &Formatter::_cname, std::tuple<std::string> { longName } };
}

FormatToken<Model::ParameterRef> Formatter::param(Model::ParameterRef parameter) const
{
    return FormatToken<Model::ParameterRef> { this, &Formatter::_param, std::tuple<Model::ParameterRef> { parameter } };
}

FormatToken<Model::TypeRef> Formatter::type(Model::TypeBaseRef type) const
{
    auto resolvedType = std::dynamic_pointer_cast<Model::Type>(type);

    if (!resolvedType)
    {
        throw std::runtime_error("cannot format unresolved type");
    }

    return FormatToken<Model::TypeRef> { this, &Formatter::_type, std::tuple<Model::TypeRef> { resolvedType } };
}

FormatToken<Model::ElementRef, std::vector<Model::ElementRef>> Formatter::type(Model::ElementRef primary, std::vector<Model::ElementRef> params) const
{
    return FormatToken<Model::ElementRef, std::vector<Model::ElementRef>> { this, &Formatter::_type, std::tuple<Model::ElementRef, std::vector<Model::ElementRef>> { primary, params } };
}

FormatToken<Model::PrimitiveRef, std::vector<Model::ElementRef>> Formatter::type(Model::PrimitiveRef primitive, std::vector<Model::ElementRef> params) const
{
    return FormatToken<Model::PrimitiveRef, std::vector<Model::ElementRef>> { this, &Formatter::_type, std::tuple<Model::PrimitiveRef, std::vector<Model::ElementRef>> { primitive, params } };
}

FormatToken<Model::DocumentationRef> Formatter::doc(Model::DocumentationRef documentation) const
{
    return FormatToken<Model::DocumentationRef> { this, &Formatter::_doc, std::tuple<Model::DocumentationRef> { documentation } };
}

FormatToken<Model::RootRef> Formatter::definition(Model::RootRef root) const
{
	return FormatToken<Model::RootRef> { this, &Formatter::_definition, std::tuple<Model::RootRef> { root } };
}

FormatToken<Model::NamespaceRef> Formatter::definition(Model::NamespaceRef namespace_) const
{
    return FormatToken<Model::NamespaceRef> { this, &Formatter::_definition, std::tuple<Model::NamespaceRef> { namespace_ } };
}

FormatToken<Model::ElementRef> Formatter::definition(Model::ElementRef element) const
{
	return FormatToken<Model::ElementRef> { this, &Formatter::_definition, std::tuple<Model::ElementRef> { element } };
}

FormatToken<Model::StructRef> Formatter::definition(Model::StructRef struct_) const
{
	return FormatToken<Model::StructRef> { this, &Formatter::_definition, std::tuple<Model::StructRef> { struct_ } };
}

FormatToken<Model::ClassRef> Formatter::definition(Model::ClassRef class_) const
{
	return FormatToken<Model::ClassRef> { this, &Formatter::_definition, std::tuple<Model::ClassRef> { class_ } };
}

FormatToken<Model::Class::ConstantRef> Formatter::definition(Model::Class::ConstantRef constant) const
{
    return FormatToken<Model::Class::ConstantRef> { this, &Formatter::_definition, std::tuple<Model::Class::ConstantRef> { constant } };
}

FormatToken<Model::Class::EventRef> Formatter::definition(Model::Class::EventRef event) const
{
	return FormatToken<Model::Class::EventRef> { this, &Formatter::_definition, std::tuple<Model::Class::EventRef> { event } };
}

FormatToken<Model::Class::OperationRef> Formatter::definition(Model::Class::OperationRef operation) const
{
	return FormatToken<Model::Class::OperationRef> { this, &Formatter::_definition, std::tuple<Model::Class::OperationRef> { operation } };
}

FormatToken<Model::Class::OperationRef> Formatter::signature(Model::Class::OperationRef operation) const
{
	return FormatToken<Model::Class::OperationRef> { this, &Formatter::_signature, std::tuple<Model::Class::OperationRef> { operation } };
}

FormatToken<Model::EnumRef> Formatter::definition(Model::EnumRef enum_) const
{
    return FormatToken<Model::EnumRef> { this, &Formatter::_definition, std::tuple<Model::EnumRef> { enum_ } };
}

FormatToken<Model::Enum::ValueRef> Formatter::definition(Model::Enum::ValueRef value) const
{
    return FormatToken<Model::Enum::ValueRef> { this, &Formatter::_definition, std::tuple<Model::Enum::ValueRef> { value } };
}

void Formatter::_qname(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( auto parent = std::dynamic_pointer_cast<Model::Identifiable>(identifiable->parent()) )
    {
        stream << qname(parent) << config.namespaceDelimiter;
    }

    stream << name(identifiable);
}

void Formatter::_name(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( std::dynamic_pointer_cast<Model::Namespace>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Namespace>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Parameter>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Parameter>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Enum>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Enum>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Enum::Value>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Enum::Value>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Struct>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Struct>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Constant>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class::Constant>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Event>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class::Event>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Operation>(identifiable) )
    {
        std::string longName = identifiable->longName(), shortName = identifiable->shortName();

        for( auto stripVerb : config.stripVerbs )
        {
            bool stripped = false;

            if( longName.find(stripVerb) == 0 )
            {
                longName = longName.substr(stripVerb.length());
                stripped = true;
            }

            if( shortName.find(stripVerb) == 0 )
            {
                shortName = shortName.substr(stripVerb.length());
                stripped = true;
            }

            if(stripped)
                { break; }
        }

        stream << name(longName, shortName, config.nameConfig<Model::Class::Operation>());
    }
    else
    {
        throw std::runtime_error("unknown identifiable type " + identifiable->typeName());
    }
}

void Formatter::_name(std::ostream& stream, std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const
{
    boost::regex regex("[A-Z]?[a-z]+|[A-Z]?[0-9]+|[A-Z]");

    std::string name = config.useShort ? shortName : longName;
    
    std::string::const_iterator start = name.begin();
    std::string::const_iterator end = name.end();

    boost::smatch match;
    for(size_t count = 0; boost::regex_search(start, end, match, regex); ++count)
    {
        if (count > 0)
            { stream << config.delimiter; }

        std::string part = match[0];

        switch (config.style)
        {
            case FormatterConfig::NameStyle::LOWER_CAMELCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                if (count > 0)
                {
                    transform(part.begin(), part.begin() + 1, part.begin(), ::toupper);
                }
                break;

            case FormatterConfig::NameStyle::UPPER_CAMELCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                transform(part.begin(), part.begin() + 1, part.begin(), ::toupper);
                break;

            case FormatterConfig::NameStyle::UPPERCASE:
                transform(part.begin(), part.end(), part.begin(), ::toupper);
                break;

            case FormatterConfig::NameStyle::LOWERCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                break;

            default:
                throw std::runtime_error("invalid name style");
        }

        stream << part;
        start = match[0].second;
    }
}

void Formatter::_qcname(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( auto parent = std::dynamic_pointer_cast<Model::Identifiable>(identifiable->parent()) )
    {
        stream << qcname(parent) << "::";
    }

    stream << cname(identifiable);
}

void Formatter::_cname(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    stream << cname(identifiable->longName());
}

void Formatter::_cname(std::ostream& stream, std::string longName) const
{
    stream << longName;
}

void Formatter::_type(std::ostream& stream, Model::TypeRef type_) const
{
    stream << type(type_->primary(), type_->params());
}

void Formatter::_type(std::ostream& stream, Model::ElementRef primary, std::vector<Model::ElementRef> params) const
{
    if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(primary))
    {
        stream << type(primitive, params);
    }
    else
    {
        if(params.size() > 0)
            { throw std::runtime_error("type parameters not supported"); }

        stream << qname(primary);
    }
}

void Formatter::_type(std::ostream& stream, Model::PrimitiveRef primitive, std::vector<Model::ElementRef> params) const
{
    if(params.size() != primitive->underlyingParamCount())
        throw std::runtime_error("unexpected count of type parameters");

    std::string native;

    switch(primitive->underlying())
    {
        case Model::Primitive::Underlying::BYTE:
            native = config.primitiveConfig1<Model::Primitive::Underlying::BYTE>().native;
            break;

        case Model::Primitive::Underlying::UINT16:
            native = config.primitiveConfig1<Model::Primitive::Underlying::UINT16>().native;
            break;

        case Model::Primitive::Underlying::UINT32:
            native = config.primitiveConfig1<Model::Primitive::Underlying::UINT32>().native;
            break;

        case Model::Primitive::Underlying::UINT64:
            native = config.primitiveConfig1<Model::Primitive::Underlying::UINT64>().native;
            break;

        case Model::Primitive::Underlying::BOOLEAN:
            native = config.primitiveConfig1<Model::Primitive::Underlying::BOOLEAN>().native;
            break;

        case Model::Primitive::Underlying::TIMESTAMP:
            native = config.primitiveConfig1<Model::Primitive::Underlying::TIMESTAMP>().native;
            break;

        case Model::Primitive::Underlying::STRING:
            native = config.primitiveConfig1<Model::Primitive::Underlying::STRING>().native;
            break;

        case Model::Primitive::Underlying::UUID:
            native = config.primitiveConfig1<Model::Primitive::Underlying::UUID>().native;
            break;

        case Model::Primitive::Underlying::BUFFER:
            native = config.primitiveConfig2<Model::Primitive::Underlying::BUFFER>().native;
            break;

        case Model::Primitive::Underlying::CONST_BUFFER:
            native = config.primitiveConfig2<Model::Primitive::Underlying::CONST_BUFFER>().native;
            break;

        case Model::Primitive::Underlying::VECTOR:
            native = config.primitiveConfig3<Model::Primitive::Underlying::VECTOR>().native;
            break;

        case Model::Primitive::Underlying::LIST:
            native = config.primitiveConfig3<Model::Primitive::Underlying::LIST>().native;
            break;

        case Model::Primitive::Underlying::SET:
            native = config.primitiveConfig3<Model::Primitive::Underlying::SET>().native;
            break;

        case Model::Primitive::Underlying::MAP:
            native = config.primitiveConfig3<Model::Primitive::Underlying::MAP>().native;
            break;

        default:
            throw std::runtime_error("invalid primitive");
    }

    std::size_t i = 0;

    for( auto param : params )
    {
        std::string f = std::string("$") + std::to_string(i);
        auto j = native.find(f);

        if( j != std::string::npos )
        {
            std::stringstream p;
            p << type(param, std::vector<Model::ElementRef>());
            native.replace(j, f.length(), p.str() );
        }

        ++i;
    }

    stream << native;
}

void Formatter::_doc(std::ostream& stream, Model::DocumentationRef documentation) const
{
	using namespace std;
	using namespace Model;
	using namespace StreamFilter;

	stream << "/**" << endl;
	
    if (documentation->keyExists(Documentation::KEY_BRIEF))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << "@" << Documentation::KEY_BRIEF << " " << documentation->description(Documentation::KEY_BRIEF) << endl;
    }

    if (documentation->keyExists(Documentation::KEY_MORE))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << endl << documentation->description(Documentation::KEY_MORE) << endl;
    }

	stream << " */" << endl;
}

void Formatter::_definition(std::ostream& stream, Model::RootRef root) const
{
    stream << definition(root->getNamespace());
}

void Formatter::_definition(std::ostream& stream, Model::ElementRef element) const
{
    if ( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        stream << definition(namespace_);
    }
    else
    if ( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(element) )
    {
        stream << definition(struct_);
    }
    else
    if ( auto enum_ = std::dynamic_pointer_cast<Model::Enum>(element) )
    {
        stream << definition(enum_);
    }
    else
    if ( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        stream << definition(class_);
    }
    else
    if ( std::dynamic_pointer_cast<Model::Primitive>(element) )
    {
        // primitives do not have definitions
    }
    else
    {
        throw std::runtime_error("unknown namespace element type " + element->typeName());
    }
}

} } // namespace: Everbase::InterfaceCompiler
