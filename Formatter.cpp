#include "Formatter.hpp"

namespace Everbase { namespace InterfaceCompiler {

void Formatter::execute ( const ConfigProvider& cprov, std::shared_ptr<Model::Root> input, std::ostream& output ) const
{
	output << format(input);
}

FormatToken<Model::IdentifiableRef> Formatter::formatName(Model::IdentifiableRef identifiable) const
{
	return FormatToken<Model::IdentifiableRef> { this, &Formatter::formatName, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<Model::RootRef> Formatter::format(Model::RootRef root) const
{
	return FormatToken<Model::RootRef> { this, &Formatter::format, std::tuple<Model::RootRef> { root } };
}

FormatToken<Model::TypeRef> Formatter::format(Model::TypeRef type) const
{
	return FormatToken<Model::TypeRef> { this, &Formatter::format, std::tuple<Model::TypeRef> { type } };
}

FormatToken<Model::ClassRef> Formatter::format(Model::ClassRef class_) const
{
	return FormatToken<Model::ClassRef> { this, &Formatter::format, std::tuple<Model::ClassRef> { class_ } };
}

FormatToken<Model::OperationRef> Formatter::format(Model::OperationRef operation) const
{
	return FormatToken<Model::OperationRef> { this, &Formatter::format, std::tuple<Model::OperationRef> { operation } };
}

FormatToken<Model::OperationRef> Formatter::formatSig(Model::OperationRef operation) const
{
	return FormatToken<Model::OperationRef> { this, &Formatter::formatSig, std::tuple<Model::OperationRef> { operation } };
}

} } // namespace: Everbase::InterfaceCompiler
