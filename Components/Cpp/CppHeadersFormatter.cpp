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
using NameConfigs = FormatterConfig::NameConfigs;

CppHeadersFormatter::CppHeadersFormatter(std::istream &configStream)
    : Formatter(FormatterConfig
        {
            std::string("::"), std::string(4, ' '), 85,
            NameConfigs {
                NameConfig<Namespace>        { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Parameter>        { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Struct>           { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum>             { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum::Value>      { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class>            { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Constant>  { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class::Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Operation> { NameStyle::LOWER_CAMELCASE, "", false }
            }
        })
    , _langConfig(configStream)
{
    _langConfig.parseTypeMap();
}

void CppHeadersFormatter::param(std::ostream& stream, Model::ParameterRef parameter) const
{
    stream << type(parameter->type()) << " " << name(parameter);
}

void CppHeadersFormatter::type(std::ostream& stream, Model::TypeRef type) const
{
    if (auto primitive = std::dynamic_pointer_cast<Primitive>(type->primary()))
    {
        stream << _langConfig.primitiveToLang(primitive);
    }
    else
    {
        stream << qname(type->primary());
    }
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    stream << "namespace " << name(namespace_) << endl << "{" << endl;

    for ( auto element : namespace_->elements() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indentData).push<counter>(count) << definition(element) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "}" << endl;
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::StructRef struct_) const
{
    stream << "struct " << name(struct_) << endl << "{" << endl;

    for (auto field : struct_->fields())
    {
        filter(stream).push<indent>(config.indentData) << param(field) << ";" << endl;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::ClassRef class_) const
{
    stream << "class " << name(class_) << endl << "{" << endl;

    for( auto operation : class_->operations() )
    {
        filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
    }

    for ( auto event : class_->events() )
    {
        filter(stream).push<indent>(config.indentData) << definition(event) << endl;
    }

    for ( auto constant : class_->constants() )
    {
        filter(stream).push<indent>(config.indentData) << definition(constant) << endl;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::Class::ConstantRef constant) const
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

    stream << "static constexpr " << type(constant->type()) << " " << name(constant) << " = " << valueString << ";" << endl;
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::Class::EventRef event) const
{
    for (auto value : event->values())
    {
        stream << type(value->type()) << " " << name(event) << "();" << endl;
    }
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation) << ";" << endl;
}

void CppHeadersFormatter::signature(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->result())
    {
        stream << type(operation->result()->type());
    }
    else
    {
        stream << "void";
    }

    stream << " " << name(operation) << "(";

    for (auto parameter : indices(operation->params()))
    {
        stream << param(parameter.value()) << (!parameter.last() ? ", " : "");
    }

    stream << ")";
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::EnumRef enum_) const
{
    stream << "enum class " << name(enum_) << endl << "{" << endl;

    for (auto value : indices(enum_->values()))
    {
        filter(stream).push<indent>(config.indentData) << definition(value.value()) << (!value.last() ? "," : "") << endl;
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    stream << name(value) << " = " << value->value();
}

} } } // namespace: Everbase::InterfaceCompiler::Components
