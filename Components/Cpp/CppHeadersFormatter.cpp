#include "CppHeadersFormatter.hpp"

#include <set>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

using NameStyle = FormatterConfig::NameStyle;
template <typename T> using NameConfig = FormatterConfig::TypeNameConfig<T>;
using Naming = FormatterConfig::Naming;

CppHeadersFormatter::CppHeadersFormatter(std::istream &configStream)
    : Formatter(FormatterConfig
        {
            std::string("::"), std::string(4, ' '), 85,
            Naming {
                NameConfig<Namespace> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Parameter> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Enum>      { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Value>     { NameStyle::UPPERCASE, "_", false },
                NameConfig<Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Struct>    { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Operation> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Constant>  { NameStyle::UPPERCASE, "", false }
            }
        })
    , _langConfig(configStream)
{
    _langConfig.parseTypeMap();
}

void CppHeadersFormatter::format(std::ostream& stream, Model::RootRef root) const
{
    stream << format(root->getNamespace());
}

void CppHeadersFormatter::format(std::ostream& stream, Model::TypeRef type) const
{
    if (auto primitive = std::dynamic_pointer_cast<Primitive>(type->primary()))
    {
        stream << _langConfig.primitiveToLang(primitive);
    }
    else if (std::dynamic_pointer_cast<Container>(type->primary()))
    {
        stream << _langConfig.containerTypeToLang(type, true);
    }
    else
    {
        stream << formatQualifiedName(type->primary());
    }
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
    stream << format(parameter->type()) << " " << formatName(parameter);
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ConstantRef constant) const
{
    int number = 0;
    std::string valueString;
    boost::uuids::uuid uuid;

    if (constant->value().type() == typeid(int))
    {
        number = boost::any_cast<int>(constant->value());
        valueString = boost::lexical_cast<std::string>(number);
    }
    else if (constant->value().type() == typeid(std::string))
    {
        valueString = "\"" + boost::any_cast<std::string>(constant->value()) + "\"";
    }
    else if (constant->value().type() == typeid(boost::uuids::uuid))
    {
        uuid = boost::any_cast<boost::uuids::uuid>(constant->value());
        valueString = boost::lexical_cast<std::string>(uuid);
    }

    stream << "static constexpr " << format(constant->type()) << " " << formatName(constant) << " = " << valueString << ";" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::StructRef struct_) const
{
    stream << "struct " << formatName(struct_) << endl << "{" << endl;

    for (auto field : struct_->fields())
    {
        filter(stream).push<indent>(config.indent) << format(field) << ";" << endl;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
    stream << "class " << formatName(class_) << endl << "{" << endl;

    for( auto operation : class_->operations() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indent).push<counter>(count) << format(operation) << (count > 0 ? "\n" : "") << flush;
    }

    for ( auto event : class_->events() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indent).push<counter>(count) << format(event) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::EventRef event) const
{
    for (auto value : event->values())
    {
        stream << format(value->type()) << " " << formatName(event) << "();" << endl;
    }
}

void CppHeadersFormatter::format(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    stream << "namespace " << formatName(namespace_) << endl << "{" << endl;

    for ( auto member : namespace_->members() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indent).push<counter>(count) << format(member) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "}" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
    stream << "enum class " << formatName(enum_) << endl << "{" << endl;

    for (auto value : indices(enum_->values()))
    {
        filter(stream).push<indent>(config.indent) << format(value.value()) << (!value.last() ? "," : "") << endl;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ValueRef value) const
{
    stream << formatName(value) << " = " << value->value();
}

void CppHeadersFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << format(operation->doc());
    }

    stream << formatSig(operation) << ";" << endl;
}

void CppHeadersFormatter::formatSig(std::ostream& stream, Model::OperationRef operation) const
{
    if (operation->result())
    {
        stream << format(operation->result()->type());
    }
    else
    {
        stream << "void";
    }

    stream << " " << formatName(operation) << "(";

    for (auto parameter : indices(operation->params()))
    {
        stream << format(parameter.value()) << (!parameter.last() ? ", " : "");
    }

    stream << ")";
}


} } } // namespace: Everbase::InterfaceCompiler::Components
