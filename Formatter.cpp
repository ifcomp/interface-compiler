#include "Formatter.hpp"

#include <boost/regex.hpp>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

Formatter::Formatter(FormatterConfig config)
    : config(config)
{
}

void Formatter::execute ( const ConfigProvider& cprov, Model::RootRef input, std::ostream& output ) const
{
	output << format(input);
}

FormatToken<Model::RootRef> Formatter::format(Model::RootRef root) const
{
	return FormatToken<Model::RootRef> { this, &Formatter::format, std::tuple<Model::RootRef> { root } };
}

FormatToken<Model::IdentifiableRef> Formatter::formatName(Model::IdentifiableRef identifiable) const
{
	return FormatToken<Model::IdentifiableRef> { this, &Formatter::formatName, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<std::string, std::string, FormatterConfig::NameConfig> Formatter::formatName(std::string longName, std::string shortName, FormatterConfig::NameConfig config) const
{
    return FormatToken<std::string, std::string, FormatterConfig::NameConfig> { this, &Formatter::formatName, std::tuple<std::string, std::string, FormatterConfig::NameConfig> { longName, shortName, config } };
}

FormatToken<Model::DocumentationRef> Formatter::format(Model::DocumentationRef documentation) const
{
	return FormatToken<Model::DocumentationRef> { this, &Formatter::format, std::tuple<Model::DocumentationRef> { documentation } };
}

FormatToken<Model::TypeRef> Formatter::format(Model::TypeBaseRef type) const
{
	auto resolvedType = std::dynamic_pointer_cast<Model::Type>(type);

    if (!resolvedType)
    {
		throw std::runtime_error("cannot format unresolved type");
    }

	return FormatToken<Model::TypeRef> { this, &Formatter::format, std::tuple<Model::TypeRef> { resolvedType } };
}

FormatToken<Model::ParameterRef> Formatter::format(Model::ParameterRef parameter) const
{
	return FormatToken<Model::ParameterRef> { this, &Formatter::format, std::tuple<Model::ParameterRef> { parameter } };
}

FormatToken<Model::NamespaceMemberRef> Formatter::format(Model::NamespaceMemberRef member) const
{
	return FormatToken<Model::NamespaceMemberRef> { this, &Formatter::format, std::tuple<Model::NamespaceMemberRef> { member } };
}

FormatToken<Model::PrimitiveRef> Formatter::format(Model::PrimitiveRef primitive) const
{
	return FormatToken<Model::PrimitiveRef> { this, &Formatter::format, std::tuple<Model::PrimitiveRef> { primitive } };
}

FormatToken<Model::ContainerRef> Formatter::format(Model::ContainerRef container) const
{
	return FormatToken<Model::ContainerRef> { this, &Formatter::format, std::tuple<Model::ContainerRef> { container } };
}

FormatToken<Model::ConstantRef> Formatter::format(Model::ConstantRef constant) const
{
	return FormatToken<Model::ConstantRef> { this, &Formatter::format, std::tuple<Model::ConstantRef> { constant } };
}

FormatToken<Model::StructRef> Formatter::format(Model::StructRef struct_) const
{
	return FormatToken<Model::StructRef> { this, &Formatter::format, std::tuple<Model::StructRef> { struct_ } };
}

FormatToken<Model::ClassRef> Formatter::format(Model::ClassRef class_) const
{
	return FormatToken<Model::ClassRef> { this, &Formatter::format, std::tuple<Model::ClassRef> { class_ } };
}

FormatToken<Model::EventRef> Formatter::format(Model::EventRef event) const
{
	return FormatToken<Model::EventRef> { this, &Formatter::format, std::tuple<Model::EventRef> { event } };
}

FormatToken<Model::NamespaceRef> Formatter::format(Model::NamespaceRef namespace_) const
{
	return FormatToken<Model::NamespaceRef> { this, &Formatter::format, std::tuple<Model::NamespaceRef> { namespace_ } };
}

FormatToken<Model::EnumRef> Formatter::format(Model::EnumRef enum_) const
{
	return FormatToken<Model::EnumRef> { this, &Formatter::format, std::tuple<Model::EnumRef> { enum_ } };
}

FormatToken<Model::ValueRef> Formatter::format(Model::ValueRef value) const
{
	return FormatToken<Model::ValueRef> { this, &Formatter::format, std::tuple<Model::ValueRef> { value } };
}

FormatToken<Model::OperationRef> Formatter::format(Model::OperationRef operation) const
{
	return FormatToken<Model::OperationRef> { this, &Formatter::format, std::tuple<Model::OperationRef> { operation } };
}

FormatToken<Model::OperationRef> Formatter::formatSig(Model::OperationRef operation) const
{
	return FormatToken<Model::OperationRef> { this, &Formatter::formatSig, std::tuple<Model::OperationRef> { operation } };
}

void Formatter::formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( std::dynamic_pointer_cast<Model::Namespace>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Namespace>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Parameter>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Parameter>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Enum>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Enum>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Value>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Value>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Event>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Event>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Struct>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Struct>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Operation>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Operation>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Constant>(identifiable) )
    {
        stream << formatName(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Constant>());
    }
    else
    {
        throw std::runtime_error("unknown identifiable type " + identifiable->objectTypeName());
    }
}

void Formatter::formatName(std::ostream& stream, std::string longName, std::string shortName, FormatterConfig::NameConfig config) const
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

void Formatter::format(std::ostream& stream, Model::DocumentationRef documentation) const
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

void Formatter::format(std::ostream& stream, Model::NamespaceMemberRef member) const
{
    if ( auto doc = member->doc() )
    {
        stream << format(doc);
    }

    if ( auto primitive = std::dynamic_pointer_cast<Model::Primitive>(member) )
    {
        stream << format(primitive);
    }
    else
    if ( auto container = std::dynamic_pointer_cast<Model::Container>(member) )
    {
        stream << format(container);
    }
    else
    if ( auto constant = std::dynamic_pointer_cast<Model::Constant>(member) )
    {
        stream << format(constant);
    }
    else
    if ( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(member) )
    {
        stream << format(struct_);
    }
    else
    if ( auto class_ = std::dynamic_pointer_cast<Model::Class>(member) )
    {
        stream << format(class_);
    }
    else
    if ( auto event = std::dynamic_pointer_cast<Model::Event>(member) )
    {
        stream << format(event);
    }
    else
    if ( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(member) )
    {
        stream << format(namespace_);
    }
    else
    if ( auto enum_ = std::dynamic_pointer_cast<Model::Enum>(member) )
    {
        stream << format(enum_);
    }
    else
    if ( auto operation = std::dynamic_pointer_cast<Model::Operation>(member) )
    {
        stream << format(operation);
    }
    else
    {
        throw std::runtime_error("unknown namespace member type " + member->objectTypeName());
    }
}

} } // namespace: Everbase::InterfaceCompiler
