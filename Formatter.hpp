#pragma once

#include "Model/Model.hpp"
#include "ConfigProvider.hpp"
#include "FormatToken.hpp"
#include "IndexList.hpp"
#include "StreamFilter.hpp"
#include "FormatterConfig.hpp"

#include <memory>
#include <ostream>

namespace Everbase { namespace InterfaceCompiler {

class Formatter
{
public:
	Formatter(FormatterConfig config);
	void execute ( const ConfigProvider& cprov, Model::RootRef input, std::ostream& output ) const;

protected:
	const FormatterConfig config;

protected:
	FormatToken<Model::RootRef> format(Model::RootRef root) const;

	FormatToken<Model::IdentifiableRef> formatQualifiedName(Model::IdentifiableRef identifiable) const;
	FormatToken<Model::IdentifiableRef> formatName(Model::IdentifiableRef identifiable) const;
	FormatToken<std::string, std::string, FormatterConfig::NameConfig> formatName(std::string longName, std::string shortName, FormatterConfig::NameConfig config) const;

	FormatToken<Model::DocumentationRef> format(Model::DocumentationRef documentation) const;
	FormatToken<Model::TypeRef> format(Model::TypeBaseRef type) const;
	FormatToken<Model::ParameterRef> format(Model::ParameterRef parameter) const;

	FormatToken<Model::NamespaceMemberRef> format(Model::NamespaceMemberRef member) const;
	FormatToken<Model::PrimitiveRef> format(Model::PrimitiveRef primitive) const;
	FormatToken<Model::ContainerRef> format(Model::ContainerRef container) const;
	FormatToken<Model::ConstantRef> format(Model::ConstantRef constant) const;
	FormatToken<Model::StructRef> format(Model::StructRef struct_) const;
	FormatToken<Model::ClassRef> format(Model::ClassRef class_) const;
	FormatToken<Model::EventRef> format(Model::EventRef event) const;
	FormatToken<Model::NamespaceRef> format(Model::NamespaceRef namespace_) const;

	FormatToken<Model::EnumRef> format(Model::EnumRef enum_) const;
	FormatToken<Model::ValueRef> format(Model::ValueRef value) const;

	FormatToken<Model::OperationRef> format(Model::OperationRef operation) const;
	FormatToken<Model::OperationRef> formatSig(Model::OperationRef operation) const;

protected:
	virtual void format(std::ostream& stream, Model::RootRef root) const = 0;

	virtual void formatQualifiedName(std::ostream& stream, Model::IdentifiableRef identifiable) const;
	virtual void formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const;
	virtual void formatName(std::ostream& stream, std::string longName, std::string shortName, FormatterConfig::NameConfig config) const;

	virtual void format(std::ostream& stream, Model::DocumentationRef documentation) const;
	virtual void format(std::ostream& stream, Model::TypeRef type) const = 0;
	virtual void format(std::ostream& stream, Model::ParameterRef parameter) const = 0;

	virtual void format(std::ostream& stream, Model::NamespaceMemberRef member) const;
	virtual void format(std::ostream& stream, Model::PrimitiveRef primitive) const;
	virtual void format(std::ostream& stream, Model::ContainerRef container) const;
	virtual void format(std::ostream& stream, Model::ConstantRef constant) const = 0;
	virtual void format(std::ostream& stream, Model::StructRef struct_) const = 0;
	virtual void format(std::ostream& stream, Model::ClassRef class_) const = 0;
	virtual void format(std::ostream& stream, Model::EventRef event) const = 0;
	virtual void format(std::ostream& stream, Model::NamespaceRef namespace_) const = 0;

	virtual void format(std::ostream& stream, Model::EnumRef enum_) const = 0;
	virtual void format(std::ostream& stream, Model::ValueRef value) const = 0;

	virtual void format(std::ostream& stream, Model::OperationRef operation) const = 0;
	virtual void formatSig(std::ostream& stream, Model::OperationRef operation) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
