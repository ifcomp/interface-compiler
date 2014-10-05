#include "Components/JavaScript/HeaderFormatter.hpp"

#include <set>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

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

HeaderFormatter::HeaderFormatter()
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

void HeaderFormatter::_param(std::ostream& stream, Model::ParameterRef parameter) const
{
	stream << "/* " << type(parameter->type()) << " */" << " " << name(parameter);
}

void HeaderFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	stream << "var " << qname(namespace_) << " = " << qname(namespace_) << " || {  }" << endl << endl;
	
	for ( auto element : namespace_->elements() )
	{
		filter(stream) << definition(element);
	}
}

void HeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
	stream << "// struct: " << qname(struct_) << " {" << endl << endl;
    stream << qname(struct_) << "= function() {  }" << endl << endl;

	for (auto field : struct_->fields())
	{
		stream << "Object.defineProperty(" << qname(struct_) << ".prototype, '" << name(field) << "', "
		       << "{get: function() { throw new Error(\"not implemented\"); }, set: function(new" << field->longName() << ") { throw new Error(\"not implemented\"); }}); "
		       << "/* " << type(field->type()) << " */" << endl << endl;
	}

	stream << "// struct: }" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "// class: " << qname(class_) << " {" << endl << endl;

	stream << qname(class_) << " = function() { };" << endl << endl;

	if( auto super = std::dynamic_pointer_cast<Model::Type>(class_->super()) )
	{
		if (auto superClass = std::dynamic_pointer_cast<Model::Class>(super->primary()))
		{
			stream << qname(class_) << ".prototype" << " = " << "Object.create(" << qname(superClass) << ".prototype);" << endl << endl;
		}
	}
	else
	{
		stream << qname(class_) << ".prototype" << " = " << "Object.create(Everbase.Observable.prototype);" << endl << endl;
	}

	for (auto operation : class_->operations())
	{
		stream << definition(operation);
	}

	for (auto event : class_->events())
	{
		stream << definition(event);
	}

	for (auto constant : class_->constants())
	{
		stream << definition(constant);
	}

	stream << "// class: }" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
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
		valueString = "\'" + boost::any_cast<std::string>(constant->value()) + "\'";
	}
	else if (constant->value().type() == typeid(boost::uuids::uuid))
	{
    	uuid = boost::any_cast<boost::uuids::uuid>(constant->value());
		valueString = boost::lexical_cast<std::string>(uuid);
	}
	stream << qname(constant) << " = " << valueString << ";" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
	stream << "// event: " << qname(event) << " {" << endl << endl;

	stream << qname(event) << " = function() { };" << endl << endl;

	stream << qname(event) << ".prototype" << " = Object.create(Everbase.Event.prototype);" << endl << endl;

	stream << qname(event) << ".TYPE_ID = " << " \'" << boost::lexical_cast<std::string>(event->typeId()) << "\';" << endl << endl;

	for (auto value : event->values())
	{
		stream << "Object.defineProperty( " << qname(event) << ".prototype, '" 
			<< name(value) <<	"', {get: function() { throw new Error(\"not implemented\"); }, set: function("
			<< name("New" + value->longName(), "New" + value->shortName(), config.nameConfig<Model::Parameter>())
			<< " ) { throw new Error(\"not implemented\"); }}); /* " << type(value->type()) << " */" << endl << endl;
	}

	stream << "// event: }" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
	if(operation->doc())
	{
		stream << doc(operation->doc());
	}

	stream << signature(operation) << " { throw new Error(\"not implemented\"); }" << endl << endl;
}

void HeaderFormatter::_signature(std::ostream& stream, Model::Class::OperationRef operation) const
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

void HeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
	stream << "// enum: " << qname(enum_) << " {" << endl << endl;

	stream << qname(enum_) << "= {  };" << endl << endl;

	for (auto value : enum_->values())
	{
		stream << definition(value);
	} 

	stream << "// enum: }" << endl << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
	if (value->doc())
	{
		stream << doc(value->doc());
	}

	stream << qname(value) << " = " << value->value() << ";" << endl << endl;
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript