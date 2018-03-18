#include "Components/TypeScript/JsonEncoding.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace TypeScript {

using std::string;

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

const string CONVERSIONS = "everbase.rpc.jsonEncoding._conversions";
const string HANDLES = "everbase.rpc.jsonEncoding._handles";

void JsonEncoding::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	if (!namespace_->parent()->parent() && namespace_->longName() == "Everbase")
	{
		//stream << "\'use strict\';" << endl << endl;
	}

    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }

    if (!namespace_->parent()->parent())
    {
		//stream << "var " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
		if (namespace_->longName() == "Everbase")
		{
			stream << "everbase.rpc = everbase.rpc || { }" << endl << endl;
			stream << "everbase.rpc.jsonEncoding = everbase.rpc.jsonEncoding || { }" << endl << endl;
			stream << CONVERSIONS << " = " << CONVERSIONS << " || { }" << endl << endl;
			stream << HANDLES << " = " << HANDLES << " || { }" << endl << endl;
		}
    }
    else
    {
		stream << "// " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
    }

	for ( auto element : namespace_->elements() )
	{
		filter(stream) << definition(element);
	}
}


void JsonEncoding::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }

	stream << "// struct: " << qname(struct_) << " {" << endl << endl;

	filter f(stream);
	f << CONVERSIONS << "['" << qcname(struct_) << "'] = {" << endl;
	f.push<indent>( config.indentData )
		<< "encode: function(value) {" << endl;
	f.push<indent>( config.indentData )
			<< "var result = new " << qname(struct_) << "();" << endl;
			for (auto field : struct_->fields())
			{
				f << "result." << name(field) << " = ";
				f << CONVERSIONS << "['";
				_paramType(f, field);
				f << "'].encode(value['" << cname(field) << "']";
                if ( _hasContainerTypes( field ) )
                {
                    f << ", [";
                    _containerTypes(f, field);
                    f << "]";
                }
                f << ")" << endl;
			}
			f << "return result;" << endl;
	f.pop() << "}," << endl;

	f << "decode: function(value) {" << endl;
	f.push<indent>( config.indentData )
		<< "var result = new " << qname(struct_) << "();" << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << name(field) << " = " << CONVERSIONS << "['";
	    _paramType(f, field);
		f << "'].decode(value['" << cname(field) << "']";
        if ( _hasContainerTypes( field ) )
        {
            f << ", [";
            _containerTypes(f, field);
            f << "]";
        }
        f << ")" << endl;
    }
	f << "return result;" << endl;
	f.pop() << "}" << endl;
	f.pop() << "}" << endl << endl;

	stream << "// struct: }" << endl << endl;
}

void JsonEncoding::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

	stream << "// class: " << qname(class_) << " {" << endl << endl;

	filter f(stream);

	f << CONVERSIONS << "['" << qcname(class_) << "'] = {" << endl;
	f.push<indent>( config.indentData )
		<< "encode: function (classObj) {" << endl;
	f.push<indent>( config.indentData )
			<< "return classObj._handle;" << endl;
	f.pop()
		<< "}," << endl
		<< "decode: function(handle) {" << endl;
	f.push<indent>( config.indentData )
            << "// 4294967295 == uint32_t(-1), js max int is 2^53-1" << endl
            << "if (handle == 4294967295) {" << endl;
    f.push<indent>( config.indentData )
                << "return null;" << endl;
    f.pop()
            << "}" << endl
			<< "if (handle in " << HANDLES << ") {" << endl;
	f.push<indent>( config.indentData )
				<< "return " << HANDLES << "[handle]" << endl;
    f.pop()
			<< "}" << endl
			<< "var result = new " << qname(class_) << "(handle);" << endl
			<< HANDLES << "[handle] = result;" << endl
			<< "return result;" << endl;
	f.pop()
		<< "}" << endl;
	f.pop()
	<< "}" << endl << endl;

	stream << "// class: }" << endl << endl;

	for (auto event : class_->events())
	{
		stream << definition(event);
	}

	for (auto constant : class_->constants())
	{
		stream << definition(constant);
	}

}


void JsonEncoding::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }
	stream << "// event: " << qname(event) << " {" << endl << endl;

	filter f(stream);

	f << CONVERSIONS << "['" << qcname(event) << "'] = {" << endl;
	f.push<indent>( config.indentData )
		<< "decode: function(eventValues) {" << endl;
	f.push<indent>( config.indentData )
			<< "var JsEvent = new " << qname(event) << "();" << endl;
	int count = 0;
	for (auto value : event->values())
	{
		f << "JsEvent." << name(value) << " = "
		  << CONVERSIONS << "['";
		_paramType(f, value);
		f << "'].decode(eventValues[" << count << "], [ ";
		_containerTypes(f, value);
		f << " ] );" << endl;
		count++;
	}
			f << "return JsEvent" << endl;
		f.pop() << "}" << endl;
	f.pop() << "}" << endl;

	stream << "// event: }" << endl << endl;
}


void JsonEncoding::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }

	stream << "// enum: " << qname(enum_) << " {" << endl << endl;

	filter f(stream);
	f << CONVERSIONS << "['" << qcname(enum_) << "'] = {" << endl;
	f.push<indent>( config.indentData )
		<< "encode: function(value) { return value }, " << endl
		<< "decode: function(value) { return value } " << endl;
	f.pop()
		<< "}";

	stream << "// enum: }" << endl << endl;
}

void JsonEncoding::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
	if (value->doc())
	{
		stream << doc(value->doc());
	}

	stream << qname(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value()) << ";" << endl << endl;
}


void JsonEncoding::_definition(std::ostream& stream, Model::Class::OperationRef operation) const { }

void JsonEncoding::_formatRequest(std::ostream& stream, Model::Class::OperationRef operation) const { }

void JsonEncoding::_paramType(filter& f, Model::ParameterRef param) const
{
	auto paramType = std::dynamic_pointer_cast<Model::Type>(param->type())->primary();
	//if primitive
	if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(paramType))
	{
		f << primitive->underlyingName();
	}
	else
	{
		f << qcname(paramType);
	}
}

void JsonEncoding::_containerTypes(filter& f, Model::ParameterRef containerParam) const
{
	bool isFirst = false;
	//if has container-params
	if (std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params().size() > 0)
	{
		auto typeParams = std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params();
		for (auto typeParam : indices(typeParams))
		{
			//if primitive
			if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(typeParam.value()))
			{
				isFirst = typeParam.first();
				isFirst ? f << "'" << primitive->underlyingName() << "'" : f << ", '" << primitive->underlyingName() << "'";
			}
			else
			{
				typeParam.first() ? f << "'" << qcname(typeParam.value()) << "'" : f << ", '" << qcname(typeParam.value()) << "'";
			}
		}
	}
}

bool JsonEncoding::_hasContainerTypes( ParameterRef containerParam ) const
{
    return std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params().size() > 0;
}

void JsonEncoding::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const {}

} } } } // namespace: Everbase::InterfaceCompiler::Components::TypeScript