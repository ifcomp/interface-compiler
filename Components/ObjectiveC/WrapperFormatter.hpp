#pragma once

#include "Components/ObjectiveC/FormatterBase.hpp"
#include "Components/ObjectiveC/CppTypeFormatter.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace ObjectiveC {

class WrapperFormatter : public FormatterBase
{
protected:
    using FormatterBase::header;
    using FormatterBase::footer;
    using FormatterBase::includes;
    using FormatterBase::forwards;
    using FormatterBase::backwards;
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
    using FormatterBase::_header;
    using FormatterBase::_footer;
    using FormatterBase::_includes;
    using FormatterBase::_forwards;
    using FormatterBase::_backwards;
    using FormatterBase::_qname;
    using FormatterBase::_name;
    using FormatterBase::_qcname;
    using FormatterBase::_cname;
    using FormatterBase::_param;
    using FormatterBase::_type;
    using FormatterBase::_doc;
    using FormatterBase::_definition;
    using FormatterBase::_signature;

    virtual void _includes(std::ostream& stream) const override;
    virtual void _forwards(std::ostream& stream, Model::ElementRef element) const override;

    virtual void _definition(std::ostream& stream, Model::StructRef struct_) const override;

    virtual void _definition(std::ostream& stream, Model::ClassRef class_) const override;
    virtual void _definition(std::ostream& stream, Model::Class::ConstantRef constant) const override;
    virtual void _definition(std::ostream& stream, Model::Class::EventRef event) const override;
    virtual void _definition(std::ostream& stream, Model::Class::OperationRef operation) const override;

    virtual void _definition(std::ostream& stream, Model::EnumRef enum_) const override;
    virtual void _definition(std::ostream& stream, Model::Enum::ValueRef value) const override;

private:
    void _typeEncode( std::ostream& stream, Model::StructRef struct_ ) const;
    void _typeEncode( std::ostream& stream, Model::ClassRef class_ ) const;
    void _typeEncode( std::ostream& stream, Model::Class::EventRef event ) const;
    void _typeEncode( std::ostream& stream, Model::EnumRef enum_ ) const;

    CppTypeFormatter cpp;
};

} } } } // namespace: Everbase::InterfaceCompiler::Components::ObjectiveC
