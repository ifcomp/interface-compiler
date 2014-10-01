#include "CppHeadersFormatter.hpp"

#include <set>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

using std::endl;
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

void CppHeadersFormatter::format(std::ostream& stream, Model::DocumentationRef documentation) const
{
    stream << "/**" << endl;

    {
        filter f(stream);
        f.push<indent>(" * ");
        f.push<wrap>(_langConfig.configAttribute<std::uint16_t>(LangConfigReader::StyleAttribute::TEXT_WRAP, documentation));

        if (documentation->keyExists(Documentation::KEY_BRIEF))
        {
            f << "@" << Documentation::KEY_BRIEF << " ";
            f << documentation->description(Documentation::KEY_BRIEF) << endl;
        }

        if (documentation->keyExists(Documentation::KEY_MORE))
        {
            f << endl << documentation->description(Documentation::KEY_MORE) << endl;
        }
    }

    stream << " */" << endl;
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
        stream << formatName(type->primary());
    }
}


void CppHeadersFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
    stream << format(parameter->type()) << " " << formatName(parameter);
}


void CppHeadersFormatter::format(std::ostream& stream, Model::PrimitiveRef primitive) const
{
    stream << "~primitive-definition:" << formatName(primitive) << "~" << endl;
}


void CppHeadersFormatter::format(std::ostream& stream, Model::ContainerRef container) const
{
    stream << "~container-definition:" << formatName(container) << "~" << endl;
}


void CppHeadersFormatter::format(std::ostream& stream, Model::ConstantRef constant) const
{
    ///< @todo value needs more formatting
    stream << "static constexpr " << format(constant->type()) << " " << formatName(constant) << " = " << constant->value() << ";" << endl;
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

    stream << "}" << endl;
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

    stream << "}" << endl;
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
        filter(stream).push<indent>() << format(member) << endl;
    }

    stream << "}" << endl;
}


void CppHeadersFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
    stream << "enum " << formatName(enum_) << endl;
    stream << "{" << endl;

    {
        filter f(stream);
        f.push<indent>(' ', _langConfig.configAttribute<std::uint16_t>(LangConfigReader::StyleAttribute::INDENT, enum_));

        for (auto value : enum_->values())
        {
            f << formatName(value) << " = " << format(value) << endl;
        }   
    }

    stream << "}" << endl;
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


void CppHeadersFormatter::format(std::ostream& stream, Model::NamespaceMemberRef member) const
{
    if ( auto doc = member->doc() )
    {
        stream << format(doc);
    }

    if ( auto primitive = std::dynamic_pointer_cast<Model::Primitive>(member) )
    {
        stream << format(primitive);
    }
    else
    if ( auto container = std::dynamic_pointer_cast<Model::Container>(member) )
    {
        stream << format(container);
    }
    else
    if ( auto constant = std::dynamic_pointer_cast<Model::Constant>(member) )
    {
        stream << format(constant);
    }
    else
    if ( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(member) )
    {
        stream << format(struct_);
    }
    else
    if ( auto class_ = std::dynamic_pointer_cast<Model::Class>(member) )
    {
        stream << format(class_);
    }
    else
    if ( auto event = std::dynamic_pointer_cast<Model::Event>(member) )
    {
        stream << format(event);
    }
    else
    if ( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(member) )
    {
        stream << format(namespace_);
    }
    else
    if ( auto enum_ = std::dynamic_pointer_cast<Model::Enum>(member) )
    {
        stream << format(enum_);
    }
    else
    if ( auto operation = std::dynamic_pointer_cast<Model::Operation>(member) )
    {
        stream << format(operation);
    }
    else
    {
        throw std::runtime_error("unknown namespace member type " + member->objectTypeName());
    }
}


} } } // namespace: Everbase::InterfaceCompiler::Components
