#pragma once

#include "Formatter.hpp"
#include "Components/LangConfigReader.hpp"

#include <fstream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class CppHeadersFormatter : public Formatter
{
private:
    LangConfigReader _langConfig;

public:
    CppHeadersFormatter(std::istream& configStream);

protected:
    using Formatter::formatName;
    using Formatter::formatSig;
    using Formatter::format;

protected:
    virtual void format(std::ostream& stream, Model::RootRef root) const override;

    virtual void formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const override;
    virtual void format(std::ostream& stream, Model::TypeRef type) const override;
    virtual void format(std::ostream& stream, Model::ParameterRef parameter) const override;

    virtual void format(std::ostream& stream, Model::PrimitiveRef primitive) const override;
    virtual void format(std::ostream& stream, Model::ContainerRef container) const override;
    virtual void format(std::ostream& stream, Model::ConstantRef constant) const override;
    virtual void format(std::ostream& stream, Model::StructRef struct_) const override;
    virtual void format(std::ostream& stream, Model::ClassRef class_) const override;
    virtual void format(std::ostream& stream, Model::EventRef event) const override;
    virtual void format(std::ostream& stream, Model::NamespaceRef namespace_) const override;

    virtual void format(std::ostream& stream, Model::EnumRef enum_) const override;
    virtual void format(std::ostream& stream, Model::ValueRef value) const override;

    virtual void format(std::ostream& stream, Model::OperationRef operation) const override;
    virtual void formatSig(std::ostream& stream, Model::OperationRef operation) const override;
};

} } } // namespace: Everbase::InterfaceCompiler::Components
