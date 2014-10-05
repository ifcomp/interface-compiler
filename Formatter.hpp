#pragma once

#include "Model/Model.hpp"
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
	void execute ( Model::RootRef input, std::ostream& output ) const;

protected:
	const FormatterConfig config;

protected:
	FormatToken<Model::IdentifiableRef> qname(Model::IdentifiableRef identifiable) const;
	FormatToken<Model::IdentifiableRef> name(Model::IdentifiableRef identifiable) const;
	FormatToken<std::string, std::string, FormatterConfig::NameConfigBase> name(std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const;

	FormatToken<Model::ParameterRef> param(Model::ParameterRef parameter) const;
	FormatToken<Model::TypeRef> type(Model::TypeBaseRef type) const;
	FormatToken<Model::PrimitiveRef> type(Model::PrimitiveRef primitive) const;

	FormatToken<Model::DocumentationRef> doc(Model::DocumentationRef documentation) const;

	FormatToken<Model::RootRef> definition(Model::RootRef root) const;
	FormatToken<Model::NamespaceRef> definition(Model::NamespaceRef namespace_) const;

	FormatToken<Model::ElementRef> definition(Model::ElementRef element) const;
	FormatToken<Model::StructRef> definition(Model::StructRef struct_) const;
	
	FormatToken<Model::ClassRef> definition(Model::ClassRef class_) const;
	FormatToken<Model::Class::ConstantRef> definition(Model::Class::ConstantRef constant) const;
	FormatToken<Model::Class::EventRef> definition(Model::Class::EventRef event) const;
	FormatToken<Model::Class::OperationRef> definition(Model::Class::OperationRef operation) const;
	FormatToken<Model::Class::OperationRef> signature(Model::Class::OperationRef operation) const;

	FormatToken<Model::EnumRef> definition(Model::EnumRef enum_) const;
	FormatToken<Model::Enum::ValueRef> definition(Model::Enum::ValueRef value) const;

protected:
	virtual void qname(std::ostream& stream, Model::IdentifiableRef identifiable) const;
	virtual void name(std::ostream& stream, Model::IdentifiableRef identifiable) const;
	virtual void name(std::ostream& stream, std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const;

	virtual void param(std::ostream& stream, Model::ParameterRef parameter) const = 0;
	virtual void type(std::ostream& stream, Model::TypeRef type) const = 0;
	virtual void type(std::ostream& stream, Model::PrimitiveRef primitive) const;

	virtual void doc(std::ostream& stream, Model::DocumentationRef documentation) const;

	virtual void definition(std::ostream& stream, Model::RootRef root) const;
	virtual void definition(std::ostream& stream, Model::NamespaceRef namespace_) const = 0;

	virtual void definition(std::ostream& stream, Model::ElementRef element) const;
	virtual void definition(std::ostream& stream, Model::StructRef struct_) const = 0;

	virtual void definition(std::ostream& stream, Model::ClassRef class_) const = 0;
	virtual void definition(std::ostream& stream, Model::Class::ConstantRef constant) const = 0;
	virtual void definition(std::ostream& stream, Model::Class::EventRef event) const = 0;
	virtual void definition(std::ostream& stream, Model::Class::OperationRef operation) const = 0;
	virtual void signature(std::ostream& stream, Model::Class::OperationRef operation) const = 0;

	virtual void definition(std::ostream& stream, Model::EnumRef enum_) const = 0;
	virtual void definition(std::ostream& stream, Model::Enum::ValueRef value) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
