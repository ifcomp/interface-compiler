#pragma once

#include "Formatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class TestFormatter : public Formatter
{
protected:
	using Formatter::formatName;
	using Formatter::format;
	using Formatter::formatSig;

protected:
	virtual void formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const override;
	virtual void format(std::ostream& stream, Model::RootRef root) const override;
	virtual void format(std::ostream& stream, Model::TypeRef type) const override;
	virtual void format(std::ostream& stream, Model::ClassRef class_) const override;
	virtual void format(std::ostream& stream, Model::OperationRef operation) const override;
	virtual void formatSig(std::ostream& stream, Model::OperationRef operation) const override;
};

} } } // namespace: Everbase::InterfaceCompiler::Components
