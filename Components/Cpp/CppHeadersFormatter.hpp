#pragma once

#include "Formatter.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class CppHeadersFormatter : public Formatter
{
public:
    CppHeadersFormatter();

protected:
    using Formatter::qname;
    using Formatter::name;
    using Formatter::param;
    using Formatter::type;
    using Formatter::doc;
    using Formatter::definition;
    using Formatter::signature;

protected:
    virtual void _param(std::ostream& stream, Model::ParameterRef parameter) const override;
    virtual void _type(std::ostream& stream, Model::ElementRef primary, std::vector<Model::ElementRef> params) const override;

    virtual void _definition(std::ostream& stream, Model::NamespaceRef namespace_) const override;
    virtual void _definition(std::ostream& stream, Model::StructRef struct_) const override;

    virtual void _definition(std::ostream& stream, Model::ClassRef class_) const override;
    virtual void _definition(std::ostream& stream, Model::Class::ConstantRef constant) const override;
    virtual void _definition(std::ostream& stream, Model::Class::EventRef event) const override;
    virtual void _definition(std::ostream& stream, Model::Class::OperationRef operation) const override;
    virtual void _signature(std::ostream& stream, Model::Class::OperationRef operation) const override;

    virtual void _definition(std::ostream& stream, Model::EnumRef enum_) const override;
    virtual void _definition(std::ostream& stream, Model::Enum::ValueRef value) const override;
};

} } } // namespace: Everbase::InterfaceCompiler::Components
