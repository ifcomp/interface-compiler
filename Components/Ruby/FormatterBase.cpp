#include "Components/Ruby/FormatterBase.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

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
            std::string("."), std::string(4, ' '), 85,
            NameConfigs {
                NameConfig<Namespace>        { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Parameter>        { NameStyle::LOWERCASE, "_", false },
                NameConfig<Struct>           { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum>             { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Enum::Value>      { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class>            { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Constant>  { NameStyle::UPPERCASE, "_", false },
                NameConfig<Class::Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class::Operation> { NameStyle::LOWERCASE, "_", false }
            },
            PrimitiveConfigs {
                PrimitiveConfig<Primitive::Underlying::BYTE>("Byte"),
                PrimitiveConfig<Primitive::Underlying::UINT16>("UInt16"),
                PrimitiveConfig<Primitive::Underlying::UINT32>("UInt32"),
                PrimitiveConfig<Primitive::Underlying::UINT64>("UInt64"),
                PrimitiveConfig<Primitive::Underlying::BOOLEAN>("Boolean"),
                PrimitiveConfig<Primitive::Underlying::TIMESTAMP>("Timestamp"),
                PrimitiveConfig<Primitive::Underlying::STRING>("String"),
                PrimitiveConfig<Primitive::Underlying::UUID>("Uuid"),
                PrimitiveConfig<Primitive::Underlying::ID256>("Id256"),
                PrimitiveConfig<Primitive::Underlying::OBJECTID>("ObjectId<$0>"),
                PrimitiveConfig<Primitive::Underlying::VERSIONID>("VersionId<$0>"),
                PrimitiveConfig<Primitive::Underlying::TYPEID>("TypeId<$0>"),
                PrimitiveConfig<Primitive::Underlying::VECTOR>("Vector<$0>"),
                PrimitiveConfig<Primitive::Underlying::LIST>("List<$0>"),
                PrimitiveConfig<Primitive::Underlying::SET>("Set<$0>"),
                PrimitiveConfig<Primitive::Underlying::MAP>("Map<$0, $1>"),
                PrimitiveConfig<Primitive::Underlying::FILESYSTEM_PATH>("FileSystemPath")
            },
            { "Get" }
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
    stream << "/* " << type(parameter->type()) << "*/ " << name(parameter);
}

void FormatterBase::_type(std::ostream& stream, Model::ElementRef primary, std::vector<Model::ElementRef> params) const
{
    if (auto primitive = std::dynamic_pointer_cast<Primitive>(primary))
    {
        stream << type(primitive, params);
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

    stream << "// namespace: " << name(namespace_) << endl << "{" << endl;

    for ( auto element : namespace_->elements() )
    {
        std::size_t count = 0;
        filter(stream).push<indent>(config.indentData).push<counter>(count) << definition(element) << (count > 0 ? "\n" : "") << flush;
    }

    stream << "// namespace: " << name(namespace_) << endl << "}" << endl;
}

void FormatterBase::_signature(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if( operation->isStatic() )
    {
        stream << "self.";
    }

    if (operation->result())
    {
        stream << "/* " << type(operation->result()->type()) << " */ ";
    }
    else
    {
        stream << "/* void */ ";
    }

    stream << " " << name(operation) << "(";

    for (auto parameter : indices(operation->params()))
    {
        stream << param(parameter.value()) << (!parameter.last() ? ", " : "");
    }

    stream << ")";
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
