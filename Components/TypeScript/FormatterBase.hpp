#pragma once

#include "Formatter.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace TypeScript {

class FormatterBase : public Formatter
{
public:
    FormatterBase();

protected:
    using Formatter::header;
    using Formatter::footer;
    using Formatter::includes;
    using Formatter::forwards;
    using Formatter::backwards;
    using Formatter::qname;
    using Formatter::name;
    using Formatter::qcname;
    using Formatter::cname;
    using Formatter::param;
    using Formatter::type;
    using Formatter::doc;
    using Formatter::definition;
    using Formatter::signature;

protected:
    using Formatter::_header;
    using Formatter::_footer;
    using Formatter::_includes;
    using Formatter::_forwards;
    using Formatter::_backwards;
    using Formatter::_qname;
    using Formatter::_name;
    using Formatter::_qcname;
    using Formatter::_cname;
    using Formatter::_param;
    using Formatter::_type;
    using Formatter::_doc;
    using Formatter::_definition;
    using Formatter::_signature;
    
	virtual void _param(std::ostream& stream, Model::ParameterRef parameter) const override;
	virtual void _type(std::ostream& stream, Model::PrimitiveRef primitive, std::vector<Model::ElementRef> params) const override;
	virtual void _definition(std::ostream& stream, Model::NamespaceRef namespace_) const override;
	virtual void _signature(std::ostream& stream, Model::Class::OperationRef operation) const override;
};

} } } } // namespace: Everbase::InterfaceCompiler::Components::TypeScript
