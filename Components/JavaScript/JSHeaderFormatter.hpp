#pragma once

#include "Formatter.hpp"
#include "Components/LangConfigReader.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class JSHeaderFormatter : public Formatter
{
private:
	LangConfigReader _langConfigReader;

public:
    JSHeaderFormatter(std::istream& configStream);

protected:
    using Formatter::qname;
	using Formatter::name;
	using Formatter::param;
	using Formatter::type;
	using Formatter::doc;
	using Formatter::definition;
	using Formatter::signature;

protected:
	virtual void param(std::ostream& stream, Model::ParameterRef parameter) const override;
	virtual void type(std::ostream& stream, Model::TypeRef type) const override;

	virtual void definition(std::ostream& stream, Model::NamespaceRef namespace_) const override;
	virtual void definition(std::ostream& stream, Model::StructRef struct_) const override;

	virtual void definition(std::ostream& stream, Model::ClassRef class_) const override;
	virtual void definition(std::ostream& stream, Model::ConstantRef constant) const override;
	virtual void definition(std::ostream& stream, Model::EventRef event) const override;
	virtual void definition(std::ostream& stream, Model::OperationRef operation) const override;
	virtual void signature(std::ostream& stream, Model::OperationRef operation) const override;

	virtual void definition(std::ostream& stream, Model::EnumRef enum_) const override;
	virtual void definition(std::ostream& stream, Model::ValueRef value) const override;
};

} } } // namespace: Everbase::InterfaceCompiler::Components
