#include "Components/JavaScript/JsonEncoding.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace JavaScript {

using std::string;

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;


void JsonEncoding::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }
    
	string  var = namespace_->parent() ? "" : "var ";
	stream << "//" << var << " " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
	
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
	f << "TypeConversion.toJSON['" << qname(struct_) << "'] = function(value) {" << endl;
	f.push<indent>()
		<< "var result = new " << qname(struct_) << ";" << endl << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << cname(field) << " = ";
		f << "TypeConversion.toJSON['" << type(field->type()) << "'](value['" << cname(field) << "'])";
		f << endl;
	}
	f << "return result;" << endl;
	f.pop() << "}" << endl << endl;

	f << "// ----" << endl << endl;


	f << "TypeConversion.toJS['" << qname(struct_) << "'] = function(value) {" << endl;
	f.push<indent>()
		<< "var result = new " << qname(struct_) << ";" << endl << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << cname(field) << " = ";
		f << "TypeConversion.toJS['" << type(field->type()) << "'](value['" << cname(field) << "'])";
		f << endl;
	}
	f << "return result;" << endl;
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


	f << "TypeConversion.toJS['" << qname(class_) << "'] = function(handle) {" << endl;
	f.push<indent>()
	    << "if (handle in classInstanceHandles) { " << endl;
	f.push<indent>()
        << "return classInstanceHandles[handle]" << endl;
    f.pop() << "}" << endl << endl;    

	f << "var result = new " << qname(class_) << "(handle);" << endl;	

	/*	//for (auto event : class_->events())
	//{
	//    stream << definition(event);
	//}

	//for (auto constant : class_->constants())
	//{
	//	stream << definition(constant);
	//} */

	f << "classInstanceHandles[handle] = result;" << endl;
	f << "return result;" << endl;
	f.pop() << "}" << endl << endl;

	f << "// ----" << endl << endl;

	f << "TypeConversion.toJSON['" << qname(class_) << "'] = function(classObj) {" << endl;
	/*for (auto operation : class_->operations())
	{
		f << "result." << cname(operation) << " = ";
		f << "TypeConversion.toJSON['" << qcname(operation) << "'](handle['" << cname(operation) << "'])";
		f << endl;
	}*/

	f.push<indent>() << "return classObj._handle;" << endl;
	f.pop() << "}" << endl << endl;
	stream << "// class: }" << endl << endl;

	/*	//for (auto event : class_->events())
	//{
	//    stream << definition(event);
	//}

	//for (auto constant : class_->constants())
	//{
	//	stream << definition(constant);
	//}  
	*/
}


void JsonEncoding::_definition(std::ostream& stream, Model::Class::EventRef event) const 
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }
    
	stream << "// event: " << qname(event) << " {" << endl << endl;

	/*
	stream << qname(event) << " = function() { };" << endl << endl;
	stream << qname(event) << ".prototype" << " = Object.create(Everbase.Event.prototype);" << endl << endl;
	stream << qname(event) << ".TYPE_ID = ";

	auto eventTypeId = event->typeId();
    stream << "[ ";
    for( auto i : indices(std::vector<std::uint8_t>(eventTypeId.data, eventTypeId.data + 16)) )
    {
        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
    }
    stream << " ];" << endl << endl;

	for (auto value : event->values())
	{
        if ( value->doc() )
        {
            stream << doc(value->doc());
        }
    
		stream << "Object.defineProperty(" << qname(event) << ".prototype, '" << name(value)
		    << "', { get: function() { return this._" << name(value) << " }, set: function(" << name(value) 
			<< ") { this._" << name(value) << " = " << name(value) << " } } ); "
			<< "/* " << type(value->type()) << " */

	/*" << endl << endl;
	}
	*/

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
	f << "TypeConversion.toJSON['" << qname(enum_) << "'] = function(value) { return value } " << endl << endl;
	f << "TypeConversion.toJS['" << qname(enum_) << "'] = function(value) { return value } " << endl << endl;

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

void JsonEncoding::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const { }

//void FormatterBase::_signature(std::ostream& stream, Model::Class::OperationRef operation) const { }

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript
