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
	f << "TypeConversion.toJSON['" << qcname(struct_) << "'] = function(value) {" << endl;
	f.push<indent>()
		<< "var result = new " << qname(struct_) << ";" << endl << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << name(field) << " = ";
		f << "TypeConversion.toJSON['";
		_paramType(f, field);
		f << "'](value['" << cname(field) << "'])";
		f << endl;
	}
	f << "return result;" << endl;
	f.pop() << "}" << endl << endl;

	f << "// ----" << endl << endl;


	f << "TypeConversion.toJS['" << qcname(struct_) << "'] = function(value) {" << endl;
	f.push<indent>()
		<< "var result = new " << qname(struct_) << ";" << endl << endl;

	for (auto field : struct_->fields())
	{
		f << "result." << name(field) << " = ";
		f << "TypeConversion.toJS['";
		_paramType(f, field);
		f << "'](value['" << cname(field) << "'])";
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

	f << "TypeConversion.toJS['" << qcname(class_) << "'] = function(handle) {" << endl;
	f.push<indent>()
	    << "if (handle in classInstanceHandles) { " << endl;
	f.push<indent>()
        << "return classInstanceHandles[handle]" << endl;
    f.pop() << "}" << endl << endl;    

	f << "var result = new " << qname(class_) << "(handle);" << endl;	

	f << "classInstanceHandles[handle] = result;" << endl;
	f << "return result;" << endl;
	f.pop() << "}" << endl << endl;

	f << "// ----" << endl << endl;

	f << "TypeConversion.toJSON['" << qcname(class_) << "'] = function(classObj) {" << endl;

	f.push<indent>() << "return classObj._handle;" << endl;
	f.pop() << "}" << endl << endl;
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

	f << "TypeConversion.toJS['" << qcname(event) << "'] = function(event_values) {" << endl;
	f.push<indent>();
	f << "var JsEvent = new " << qname(event) << "();" << endl << endl;
	int count = 0;
	for (auto value : event->values())
	{
		f << "JsEvent." << name(value) << " = ";
		f << "TypeConversion.toJS['";
		_paramType(f, value);
		f << "'](event_values[" << count << "], [ ";
			
		_containerTypes(f, value);
			
		f << " ] );" << endl << endl;
		count++;
	}
	f << "return JsEvent" << endl;
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
	f << "TypeConversion.toJSON['" << qcname(enum_) << "'] = function(value) { return value } " << endl << endl;
	f << "TypeConversion.toJS['" << qcname(enum_) << "'] = function(value) { return value } " << endl << endl;

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

void JsonEncoding::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

	stream << "//" << qname(constant) << endl << endl;
    

    if ( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
		stream << "TypeConversion.toJS['" << qcname(constant) << "'] = function(constValue) {" << endl;
		filter f(stream);
		f.push<indent>();
		f << "return TypeConversion.toJS[ '";
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
				f << "Byte' ](" 
					<< "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
				f << "UInt16' ](" 
					<< "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
				f << "UInt32' ](" 
					<< "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
				f << "UInt64' ](" 
					<< "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
				f << "Boolean' ](" 
					<< (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
				//f << "Timestamp' ](" 
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
				f << "String' ](" 
					<< "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
					f << "Uuid' ](";
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    f << "[ ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        f << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    f << " ]";
                }
                break;

			default:
			{
				f << " ](";
                throw std::runtime_error("not supported");
			};
			f << ");";
        }
    }
	stream << ";" << endl << endl;
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
	if (std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params().size())
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

} } } } // namespace: Everbase::InterfaceCompiler::Components::JavaScript
