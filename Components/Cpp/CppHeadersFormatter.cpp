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

CppHeadersFormatter::CppHeadersFormatter(std::istream &configStream)
    : _langConfig(configStream)
{
    _langConfig.parseTypeMap();
}

void CppHeadersFormatter::format(std::ostream& stream, Model::RootRef root) const
{
    stream << format(root->getNamespace()) << endl;
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
        stream << _langConfig.formatNamespace(type->primary()) << formatName(type->primary());
    }
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
    stream << format(parameter->type()) << " " << formatName(parameter);
}

void CppHeadersFormatter::format(std::ostream& stream, Model::PrimitiveRef primitive) const
{
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ContainerRef container) const
{
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
    stream << "struct " << formatName(struct_) << endl
           << "{" << endl;

    {
        filter f(stream);
        f.push<indent>(' ', _langConfig.configAttribute<std::uint16_t>(LangConfigReader::StyleAttribute::INDENT, struct_));

        for (auto field : struct_->fields())
        {
            f << format(field) << ";" << endl;
        }
    }
    stream << "};" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
    stream << "class " << formatName(class_) << endl << "{" << endl;

    {
        filter f(stream);
        f.push<indent>(' ', _langConfig.configAttribute<std::uint16_t>(LangConfigReader::StyleAttribute::INDENT, class_));

        for( auto operation : class_->operations() )
        {
            f << format(operation);
        }

        if (class_->events().size())
        {
            f << endl << "// ----- Events: -----" << endl;
        }

        for ( auto event : class_->events() )
        {
            f << format(event) << endl;
        }   
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
        filter(stream).push<indent>().push<counter>(count) << format(member) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "}" << endl << endl << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
    stream << "enum class " << formatName(enum_) << endl << "{" << endl;

    {
        filter f(stream);
        f.push<indent>(' ', _langConfig.configAttribute<std::uint16_t>(LangConfigReader::StyleAttribute::INDENT, enum_));

        for (auto value : enum_->values())
        {
            f << formatName(value) << " = " << format(value) << endl;
        }   
    }

    stream << "};" << endl;
}

void CppHeadersFormatter::format(std::ostream& stream, Model::ValueRef value) const
{
    stream << value->value();
}

void CppHeadersFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << format(operation->doc());
    }

    stream << formatSig(operation) << ";" << endl;
}

void CppHeadersFormatter::formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    stream << _langConfig.styleToken(identifiable->longName(), identifiable);
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
