#include "Components/Cpp/FormatterBase.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

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
            std::string("::"), std::string(4, ' '), 85,
            NameConfigs {
                NameConfig<Namespace>        { NameStyle::LOWERCASE, "_", false },
                NameConfig<Parameter>        { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Struct>           { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum>             { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum::Value>      { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class>            { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Constant>  { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class::Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Operation> { NameStyle::LOWER_CAMELCASE, "", false }
            },
            PrimitiveConfigs {
                PrimitiveConfig<Primitive::Underlying::BYTE>("std::uint8_t"),
                PrimitiveConfig<Primitive::Underlying::UINT16>("std::uint16_t"),
                PrimitiveConfig<Primitive::Underlying::UINT32>("std::uint32_t"),
                PrimitiveConfig<Primitive::Underlying::UINT64>("std::uint64_t"),
                PrimitiveConfig<Primitive::Underlying::BOOLEAN>("bool"),
                PrimitiveConfig<Primitive::Underlying::TIMESTAMP>("std::time_t"),
                PrimitiveConfig<Primitive::Underlying::STRING>("std::string"),
                PrimitiveConfig<Primitive::Underlying::UUID>("everbase::common::Uuid"),
                PrimitiveConfig<Primitive::Underlying::ID256>("everbase::common::Id256"),
                PrimitiveConfig<Primitive::Underlying::OBJECTID>("everbase::common::ObjectId"),
                PrimitiveConfig<Primitive::Underlying::VERSIONID>("everbase::common::VersionId"),
                PrimitiveConfig<Primitive::Underlying::TYPEID>("everbase::common::TypeId"),
                PrimitiveConfig<Primitive::Underlying::VECTOR>("std::vector<$0>"),
                PrimitiveConfig<Primitive::Underlying::LIST>("std::list<$0>"),
                PrimitiveConfig<Primitive::Underlying::SET>("std::set<$0>"),
                PrimitiveConfig<Primitive::Underlying::MAP>("std::map<$0, $1>"),
                PrimitiveConfig<Primitive::Underlying::FILESYSTEM_PATH>("everbase::file_system::Path")
            },
            { "From", "Get" }
        })
{
}

void FormatterBase::_includes(std::ostream& stream) const
{
    stream << "#include <memory>" << endl
           << "#include <set>" << endl
           << "#include <map>" << endl
           << "#include <ctime>" << endl
           << "#include \"everbase/common/Id.hpp\"" << endl
           << "#include \"everbase/common/Event.hpp\"" << endl
           << "#include \"everbase/file_system/Path.hpp\"" << endl
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

        //if(class_->behavior() == Class::Behavior::VALUE)
        //{
        //    stream << qname(primary);
        //}
        //else
        //{
            stream << qname(primary) << "Ref";
        //}
    }
    else
    {
        if(params.size() > 0)
            { throw std::runtime_error("type parameters not supported"); }

        stream << qname(primary);
    }
}

void FormatterBase::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }
    
    stream << "namespace " << name(namespace_) << endl << "{" << endl;

    for ( auto element : namespace_->elements() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indentData).push<counter>(count) << definition(element) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "}" << endl;
}

void FormatterBase::_signature(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if( operation->isStatic() )
    {
        stream << "static ";
    }
    else
    {
        stream << "virtual ";
    }

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

    if( operation->isConst() )
    {
        stream << " const";
    }
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
