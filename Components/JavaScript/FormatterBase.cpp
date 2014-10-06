#include "Components/JavaScript/FormatterBase.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace JavaScript {

using std::string;

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
                PrimitiveConfig<Primitive::Underlying::BYTE>("Byte"),
                PrimitiveConfig<Primitive::Underlying::UINT16>("UInt16"),
                PrimitiveConfig<Primitive::Underlying::UINT32>("UInt32"),
                PrimitiveConfig<Primitive::Underlying::UINT64>("UInt64"),
                PrimitiveConfig<Primitive::Underlying::BOOLEAN>("Boolean"),
                PrimitiveConfig<Primitive::Underlying::TIMESTAMP>("Timestamp"),
                PrimitiveConfig<Primitive::Underlying::STRING>("String"),
                PrimitiveConfig<Primitive::Underlying::UUID>("Uuid"),
                PrimitiveConfig<Primitive::Underlying::BUFFER>("Buffer"),
                PrimitiveConfig<Primitive::Underlying::CONST_BUFFER>("ConstBuffer"),
                PrimitiveConfig<Primitive::Underlying::VECTOR>("Vector<$0>"),
                PrimitiveConfig<Primitive::Underlying::LIST>("List<$0>"),
                PrimitiveConfig<Primitive::Underlying::SET>("Set<$0>"),
                PrimitiveConfig<Primitive::Underlying::MAP>("Map<$0, $1>")
            }
        })
{
}

void FormatterBase::_param(std::ostream& stream, Model::ParameterRef parameter) const
{
	stream << "/* " << type(parameter->type()) << " */" << " " << name(parameter);
}

void FormatterBase::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }
    
	stream << "var " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
	
	for ( auto element : namespace_->elements() )
	{
		filter(stream) << definition(element);
	}
}

void FormatterBase::_signature(std::ostream& stream, Model::Class::OperationRef operation) const
{
	if(operation->result())
	{
		if (!operation->isSynchronous())
		{
			stream << "/* Promise [" << type(operation->result()->type()) << " " << name(operation->result()) << "] */";
		}
		else 
		{
			stream << "/* " << type(operation->result()->type()) <<  " " << name(operation->result()) << " */";
		}
	}
	else 
	{
		if (!operation->isSynchronous())
		{
			stream << "/* Promise [] */";
		}
		else
		{
			stream << "/* void */";
		}
	}
	
    stream << " " << qname(std::dynamic_pointer_cast<Model::Identifiable>(operation->parent())) << (!operation->isStatic() ? ".prototype." : ".") <<  name(operation) <<  " = function" << "(";

	for( auto parameter : indices(operation->params()) )
	{
		stream << param(parameter.value()) << (!parameter.last() ? ", " : "");
	}

	stream << ")";
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript
