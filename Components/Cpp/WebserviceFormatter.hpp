#pragma once

#include "Components/Cpp/FormatterBase.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

class WebserviceFormatter : public FormatterBase
{
protected:
    using FormatterBase::qname;
    using FormatterBase::name;
    using FormatterBase::qcname;
    using FormatterBase::cname;
    using FormatterBase::param;
    using FormatterBase::type;
    using FormatterBase::doc;
    using FormatterBase::definition;
    using FormatterBase::signature;

protected:
    using Formatter::_qname;
    using Formatter::_name;
    using Formatter::_qcname;
    using Formatter::_cname;
    using Formatter::_param;
    using Formatter::_type;
    using Formatter::_doc;
    using Formatter::_definition;
    using Formatter::_signature;
    
    virtual void _forwards(std::ostream& stream, Model::ElementRef element) const override;

    virtual void _definition(std::ostream& stream, Model::StructRef struct_) const override;

    virtual void _definition(std::ostream& stream, Model::ClassRef class_) const override;
    virtual void _definition(std::ostream& stream, Model::Class::ConstantRef constant) const override;
    virtual void _definition(std::ostream& stream, Model::Class::EventRef event) const override;
    virtual void _definition(std::ostream& stream, Model::Class::OperationRef operation) const override;

    virtual void _definition(std::ostream& stream, Model::EnumRef enum_) const override;
    virtual void _definition(std::ostream& stream, Model::Enum::ValueRef value) const override;
};

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
