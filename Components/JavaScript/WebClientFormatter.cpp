#include "Components/JavaScript/WebClientFormatter.hpp"

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


void WebClientFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }
    
	string var = namespace_->parent() ? "" : "var ";
	stream << var << " " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
	
	for ( auto element : namespace_->elements() )
	{
		filter(stream) << definition(element);
	}

	//Only once after rootnamespace
	if (!namespace_->parent())
	{
	//	_initializeWebSocket(stream);
	//	_generateTypeConversion(stream, namespace_);
	}
}

void WebClientFormatter::_generateTypeConversion(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	auto ta = *namespace_;
	stream << "typeConversion = { }" << endl << endl;
	stream << "typeConversion.toJSON = function() { }" << endl;
	stream << "typeConversion.toJS = function() { }" << endl;
	filter f(stream);

	for ( auto element : namespace_->elements() )
	{
		if( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(element) )
		{
			//stream << name(element->longName(), element->shortName(), config.nameConfig<Model::Struct>());
		   f << "typeConversion.toJSON['" << cname(struct_) << "'] = function(value) {" << endl;
		   f.push<indent>()
			   << "var result = new " << qname(struct_) << ";" << endl;

		   for (auto field : struct_->fields())
		   {
			   f << "result" << cname(field) << " = " << "value[" << cname(field) << "]" << endl;
		   }
		   f.pop() << "}" << endl;
		}
	    else 
        if(std::dynamic_pointer_cast<Model::PrimitiveRef>(element))
	    {
			Model::Primitive* primitive = (std::dynamic_pointer_cast<Model::PrimitiveRef>(element))->get();
			switch(primitive->underlying())
			{
				case Model::Primitive::Underlying::BYTE:
					f << "TypeConversion.toJSON['Everbase::Primitives::Byte'] = function(value) { return value; }";
					break;

				case Model::Primitive::Underlying::UINT16:
					f << "TypeConversion.toJSON['Everbase::Primitives::UINT16'] = function(value) { return value; }";
					break;

		/*		case Model::Primitive::Underlying::UINT32:
					native = config.primitiveConfig1<Model::Primitive::Underlying::UINT32>().native;
					break;

				case Model::Primitive::Underlying::UINT64:
					native = config.primitiveConfig1<Model::Primitive::Underlying::UINT64>().native;
					break;

				case Model::Primitive::Underlying::BOOLEAN:
					native = config.primitiveConfig1<Model::Primitive::Underlying::BOOLEAN>().native;
					break;

				case Model::Primitive::Underlying::TIMESTAMP:
					native = config.primitiveConfig1<Model::Primitive::Underlying::TIMESTAMP>().native;
					break;

				case Model::Primitive::Underlying::STRING:
					native = config.primitiveConfig1<Model::Primitive::Underlying::STRING>().native;
					break;

				case Model::Primitive::Underlying::UUID:
					native = config.primitiveConfig1<Model::Primitive::Underlying::UUID>().native;
					break;

				case Model::Primitive::Underlying::BUFFER:
					native = config.primitiveConfig2<Model::Primitive::Underlying::BUFFER>().native;
					break;

				case Model::Primitive::Underlying::CONST_BUFFER:
					native = config.primitiveConfig2<Model::Primitive::Underlying::CONST_BUFFER>().native;
					break;

				case Model::Primitive::Underlying::VECTOR:
					native = config.primitiveConfig3<Model::Primitive::Underlying::VECTOR>().native;
					break;

				case Model::Primitive::Underlying::LIST:
					native = config.primitiveConfig3<Model::Primitive::Underlying::LIST>().native;
					break;

				case Model::Primitive::Underlying::SET:
					native = config.primitiveConfig3<Model::Primitive::Underlying::SET>().native;
					break;

				case Model::Primitive::Underlying::MAP:
					native = config.primitiveConfig3<Model::Primitive::Underlying::MAP>().native;
					break;*/

				default:
					throw std::runtime_error("invalid primitive");
			}
	    }
	}
}

void WebClientFormatter::_initializeWebSocket(std::ostream& stream) const
{
	filter f(stream);
	f << "var processes = { };" << endl
		<< "var host = 'ws://localhost:3000';" << endl
		//<< "var ws = new WebSocket(host);" << endl << endl
		<< "var ws = { };" << endl << endl

		<< "ws.onopen = onOpen;" << endl
		<< "ws.onmessage = onMessage;" << endl
		<< "ws.onclose = onClose;" << endl
		<< "ws.onerror = onError;" << endl << endl

		<< "function onOpen(openEventArgs) { };" << endl << endl;

	f << "function onMessage(msgEventArgs) {" << endl;
	f.push<indent>()
		<< "var response = JSON.parse(msgEventArgs.data);" << endl
		<< "if (response.id in processes) { " << endl;
	f.push<indent>()
		<< "processes[response.id](response.value); " << endl
		<< "delete processes[response.id];" << endl;
	f.pop() << "}" << endl;
	f.pop() << "};" << endl << endl;


	f << "function onClose(closeEventArgs) { };" << endl << endl
		<< "function onError(errorEventArgs) { };" << endl << endl;


	//Create a GUID from randomly generated numbers.
	f << "var uuid = (function () {" << endl;
	f.push<indent>() << "function s4() {" << endl;
	f.push<indent>() << "return Math.floor((1 + Math.random()) * 0x10000)" << endl;
	f.push<indent>() << ".toString(16)" << endl
					 << ".substring(1);" << endl;
	f.pop() << "}" << endl;
	f.pop() << "return function () {" << endl;
	f.push<indent>() << "return s4() + s4() + '-' + s4() + '-' + s4() + '-' +" << endl;
	f.push<indent>() << "s4() + '-' + s4() + s4() + s4();" << endl;
	f.pop().pop() << "};" << endl;
	f.pop() << "})();";
};


void WebClientFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }
    
	stream << "// struct: " << qname(struct_) << " {" << endl << endl;
    stream << qname(struct_) << " = function() { };" << endl << endl;

	for (auto field : struct_->fields())
	{
        if ( field->doc() )
        {
            stream << doc(field->doc());
        }
    
		stream << "Object.defineProperty(" << qname(struct_) << ".prototype, '" << name(field) << "', "
		       << "{ get: function() { return this._" << name(field) << " }, set: function(" << name(field) << ") { this._" << name(field) << " = " << name(field) << " } } ); "
		       << "/* " << type(field->type()) << " */" << endl << endl;
	}

	stream << "// struct: }" << endl << endl;


	filter f(stream);
	f << "typeConversion.toJSON['" << qcname(struct_) << "'] = function(value) {" << endl;
	f.push<indent>()
		<< "var result = new " << qname(struct_) << ";" << endl << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << cname(field) << " = ";
		f << "typeConversion.toJSON['" << type(field->type()) << "'](value['" << cname(field) << "'])";
		f << endl;
	}
	f << "return result;" << endl;
	f.pop() << "}" << endl << endl;
}

void WebClientFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }
    
	stream << "// class: " << qname(class_) << " {" << endl << endl;

	stream << qname(class_) << " = function() { };" << endl << endl;

	if( auto super = std::dynamic_pointer_cast<Model::Type>(class_->super()) )
	{
		if (auto superClass = std::dynamic_pointer_cast<Model::Class>(super->primary()))
		{
			stream << qname(class_) << ".prototype" << " = " << "Object.create(" << qname(superClass) << ".prototype);" << endl << endl;
		}
	}

	for (auto constant : class_->constants())
	{
		stream << definition(constant);
	}

	for (auto operation : class_->operations())
	{
		stream << definition(operation);
	}

	for (auto event : class_->events())
	{
		stream << definition(event);
	}

	stream << "// class: }" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

	stream << qname(constant) << " = ";
    
    if ( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
                stream << "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
                stream << (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "[ ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " ]";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

	stream << ";" << endl << endl;
}

void WebClientFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }
    
	stream << "// event: " << qname(event) << " {" << endl << endl;

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
			<< "/* " << type(value->type()) << " */" << endl << endl;
	}

	stream << "// event: }" << endl << endl;
}

void WebClientFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
	if(operation->doc())
	{
		stream << doc(operation->doc());
	}

	stream << signature(operation) << " {"; 

	_formatRequest(stream, operation);

	filter f(stream);
	f.push<indent>()
		<< "ws.send(JSON.stringify(message));" << endl << endl
		<< "return new Promise(function (resolve, reject) {" << endl;

	f.push<indent>()
		<< "//if succeeded" << endl
		<< "processes[message.id] = resolve;" << endl
		<< "//if failed" << endl
		<< "//processes[message.id] = reject;" << endl;

	f.pop()
		<< "});" << endl;

	f.pop()
		<< "};" << endl;
}

void WebClientFormatter::_formatRequest(std::ostream& stream, Model::Class::OperationRef operation) const 
{
	filter f(stream);
	f << endl;
	f.push<indent>() << "var message = {" << endl;
	f.push<indent>()
		<< "type: \'call\'," << endl
		<< "id: uuid()," << endl
		<< "class: \'" << qcname(std::dynamic_pointer_cast<Model::Class>(operation->parent())) << "\'," << endl
		<< "operation: \'" << cname(operation) << "\'," << endl
		<< "parameters: {";

	if (operation->params().size())
	{
		f << endl;
		f.push<indent>();

		for (auto param : operation->params())
		{
			f << cname(param) << ": " << name(param) << "," << endl;
		}
		f.pop() << "}";
		f << endl;
	}
	else
	{
		f << " }" << endl;
	}
	f.pop() << "}";

	f << endl;
}

void WebClientFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }
    
	stream << "// enum: " << qname(enum_) << " {" << endl << endl;

	stream << qname(enum_) << " = { };" << endl << endl;

	for (auto value : enum_->values())
	{
		stream << definition(value);
	} 

	stream << "// enum: }" << endl << endl;

	filter f(stream);
	f << "typeConversion.toJSON['" << qcname(enum_) << "'] = function(value) { return value } " << endl << endl;
}

void WebClientFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
	if (value->doc())
	{
		stream << doc(value->doc());
	}

	stream << qname(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value()) << ";" << endl << endl;
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript
