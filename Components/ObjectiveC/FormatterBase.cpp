#include "Components/ObjectiveC/FormatterBase.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace ObjectiveC {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

using NameStyle = FormatterConfig::NameStyle;

template <typename T>
using NameConfig = FormatterConfig::NameConfig<T>;

template <Model::Primitive::Underlying U>
using PrimitiveConfig = FormatterConfig::PrimitiveConfig<U>;

using NameConfigs = FormatterConfig::NameConfigs;
using PrimitiveConfigs = FormatterConfig::PrimitiveConfigs;

FormatterBase::FormatterBase()
    : Formatter(FormatterConfig
        {
            std::string(""), std::string(4, ' '), 85,
            NameConfigs {
                NameConfig<Namespace>        { NameStyle::UPPERCASE, "", true },
                NameConfig<Parameter>        { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Struct>           { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum>             { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum::Value>      { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class>            { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Constant>  { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Operation> { NameStyle::LOWER_CAMELCASE, "", false }
            },
            PrimitiveConfigs {
                PrimitiveConfig<Primitive::Underlying::BYTE>("uint8_t"),
                PrimitiveConfig<Primitive::Underlying::UINT16>("uint16_t"),
                PrimitiveConfig<Primitive::Underlying::UINT32>("uint32_t"),
                PrimitiveConfig<Primitive::Underlying::UINT64>("uint64_t"),
                PrimitiveConfig<Primitive::Underlying::BOOLEAN>("bool"),
                PrimitiveConfig<Primitive::Underlying::TIMESTAMP>("time_t"),
                PrimitiveConfig<Primitive::Underlying::STRING>("NSString*"),
                PrimitiveConfig<Primitive::Underlying::UUID>("NSUUID*"),
                PrimitiveConfig<Primitive::Underlying::ID256>("evb_id256_t"),
                PrimitiveConfig<Primitive::Underlying::OBJECTID>("NSUUID*"),
                PrimitiveConfig<Primitive::Underlying::VERSIONID>("evb_id256_t"),
                PrimitiveConfig<Primitive::Underlying::TYPEID>("NSUUID*"),
                PrimitiveConfig<Primitive::Underlying::VECTOR>("NSArray*"),
                PrimitiveConfig<Primitive::Underlying::LIST>("NSArray*"),
                PrimitiveConfig<Primitive::Underlying::SET>("NSSet*"),
                PrimitiveConfig<Primitive::Underlying::MAP>("NSDictionary*"),
                PrimitiveConfig<Primitive::Underlying::FILESYSTEM_PATH>("NSString*")
            },
            { "From", "Get" }
        })
{
}

void FormatterBase::_includes(std::ostream& stream) const
{
    stream << "#include <stdint.h>" << endl
           << "#include <time.h>" << endl
           << "#include <stdbool.h>" << endl
           << "#import <Foundation/NSString.h>" << endl
           << "#import <Foundation/NSUUID.h>" << endl
           << "#import <Foundation/NSArray.h>" << endl
           << "#import <Foundation/NSSet.h>" << endl
           << "#import <Foundation/NSDictionary.h>" << endl
           << "#include \"everbase/id.h\"" << endl
           << "#include \"everbase/Event.h\"" << endl
           << endl;
}

void FormatterBase::_param(std::ostream& stream, Model::ParameterRef parameter) const
{
    stream << type(parameter->type()) << " " << name(parameter);
}

void FormatterBase::_type(std::ostream& stream, Model::ElementRef primary, std::vector<Model::ElementRef> params) const
{
    if (auto primitive = std::dynamic_pointer_cast<Primitive>(primary))
    {
        stream << type(primitive, params);
    }
    else if( auto class_ = std::dynamic_pointer_cast<Class>(primary) )
    {
        if(params.size() > 0)
            { throw std::runtime_error("type parameters not supported"); }

        stream << qname(primary) << "*";
    }
    else
    {
        if(params.size() > 0)
            { throw std::runtime_error("type parameters not supported"); }

        stream << qname(primary);
    }
}

void FormatterBase::_type(std::ostream& stream, Model::PrimitiveRef primitive, std::vector<Model::ElementRef> params) const
{
    if(params.size() != primitive->underlyingParamCount())
        throw std::runtime_error("unexpected count of type parameters");

    if(    primitive->underlying() == Model::Primitive::Underlying::VECTOR
        && std::dynamic_pointer_cast<Model::Primitive>(params[0])
        && std::dynamic_pointer_cast<Model::Primitive>(params[0])->underlying() == Model::Primitive::Underlying::BYTE )
    {
        stream << "NSData*";
    }
    else
    {
        Formatter::_type(stream, primitive, params);
    }
}

void FormatterBase::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }

    stream << "// namespace " << qcname(namespace_, ".") << ": begin" << endl << endl;

    for ( auto element : namespace_->elements() )
    {
        std::size_t count = 0;
        filter(stream).push<counter>(count) << definition(element) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "// namespace " << qcname(namespace_, ".") << ": end" << endl << endl;
}

void FormatterBase::_signature(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if( operation->isStatic() )
    {
        stream << "+ ";
    }
    else
    {
        stream << "- ";
    }

    if (operation->result())
    {
        stream << "(" << type(operation->result()->type()) << ")";
    }
    else
    {
        stream << "(void)";
    }

    stream << " " << name(operation);

    for (auto parameter : indices(operation->params()))
    {
        if(!parameter.first())
        {
            stream << name(parameter.value());
        }
        stream << ":(" << type(parameter.value()->type()) << ")" << name(parameter.value()) << (!parameter.last() ? " " : "");
    }
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::ObjectiveC
