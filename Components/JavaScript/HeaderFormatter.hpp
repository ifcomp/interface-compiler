#pragma once

#include "Components/JavaScript/FormatterBase.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace JavaScript {

class HeaderFormatter : public FormatterBase
{
protected:
    using FormatterBase::qname;
	using FormatterBase::name;
	using FormatterBase::param;
	using FormatterBase::type;
	using FormatterBase::doc;
	using FormatterBase::definition;
	using FormatterBase::signature;

protected:
    using FormatterBase::_qname;
    using FormatterBase::_name;
    using FormatterBase::_param;
    using FormatterBase::_type;
    using FormatterBase::_doc;
    using FormatterBase::_definition;
    using FormatterBase::_signature;
    
	virtual void _definition(std::ostream& stream, Model::StructRef struct_) const override;

	virtual void _definition(std::ostream& stream, Model::ClassRef class_) const override;
	virtual void _definition(std::ostream& stream, Model::Class::ConstantRef constant) const override;
	virtual void _definition(std::ostream& stream, Model::Class::EventRef event) const override;
	virtual void _definition(std::ostream& stream, Model::Class::OperationRef operation) const override;

	virtual void _definition(std::ostream& stream, Model::EnumRef enum_) const override;
	virtual void _definition(std::ostream& stream, Model::Enum::ValueRef value) const override;
};

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript
