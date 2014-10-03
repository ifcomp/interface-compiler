#include "Formatter.hpp"

namespace Everbase { namespace InterfaceCompiler {

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

} } // namespace: Everbase::InterfaceCompiler
