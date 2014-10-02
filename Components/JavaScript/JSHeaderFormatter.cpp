#include "Components/JavaScript/JSHeaderFormatter.hpp"

#include <set>
#include <iostream>
#include <string>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using std::string;

namespace Everbase { namespace InterfaceCompiler { namespace Components {

using std::endl;
using std::flush;
using IndexList::indices;
using namespace Model;
using namespace StreamFilter;

JSHeaderFormatter::JSHeaderFormatter(std::istream &configStream)
    : _langConfigReader(configStream)
{
    _langConfigReader.parseTypeMap();
}

void JSHeaderFormatter::format(std::ostream& stream, Model::RootRef root) const
{
	stream << format(root->getNamespace()) << endl;
}

void JSHeaderFormatter::formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    stream << _langConfigReader.styleToken(identifiable->longName(), identifiable);
}

void JSHeaderFormatter::format(std::ostream& stream, Model::DocumentationRef documentation) const
{
	stream << "/**" << endl;
	    
    if (documentation->keyExists(Documentation::KEY_BRIEF))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << "@" << Documentation::KEY_BRIEF << " " << documentation->description(Documentation::KEY_BRIEF) << endl;
    }

    if (documentation->keyExists(Documentation::KEY_MORE))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << endl << documentation->description(Documentation::KEY_MORE) << endl;
    }

	stream << " */" << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::TypeRef type) const
{
    if (PrimitiveRef primitive = std::dynamic_pointer_cast<Primitive>(type->primary()))
    {
		stream << _langConfigReader.primitiveToLang(primitive);
    }
    else if (std::dynamic_pointer_cast<Container>(type->primary()))
    {
		stream << _langConfigReader.containerTypeToLang(type, true);
    }
    else
    {
        stream << formatName(type->primary());
    }
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
	stream << "/*" << format(parameter->type()) << "*/" << " " << formatName(parameter);
}

void JSHeaderFormatter::format(std::ostream& stream, Model::PrimitiveRef primitive) const
{
    stream << "~primitive~" << formatName(primitive) << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ContainerRef container) const
{
    stream << "~container~" << formatName(container) << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ConstantRef constant) const
{
	stream << "~namespace~" << formatName(constant) << ".TYPE_ID =  " << " " << boost::any_cast<std::string>(constant->value()) << ";" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::StructRef struct_) const
{
	stream << "// struct " << formatName(struct_) << endl << "// {" << endl << endl;
    stream << _langConfigReader.formatNamespace(struct_) << formatName(struct_) << "= function() {  }" << endl << endl;

	for (auto field : struct_->fields())
	{
		stream << "Object.defineProperty(" << "~namespace~" << formatName(struct_) << ".prototype, '" << formatName(field) << 
			"', {get: function() { /*impl*/ }, set: function(new" << field->longName() << 
			") { /*impl*/ }}); /*" << format(field->type()) << "*/" << endl << endl;
	}

	stream << "// }" << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "// class " << formatName(class_) << endl << "// {" << endl << endl;

	stream << "~namespace~" << "." << formatName(class_) << " = function() { };";

	if( auto parent = std::dynamic_pointer_cast<Model::Class>(class_->parent()) ) {
		stream << "~namespace~" << "." << "prototype" << "Object.create(" << "~parent_namespace~" << formatName(parent) << ".prototype);";
	}

	stream << endl << endl;

	for (auto operation : class_->operations())
	{
		stream << format(operation);
	}

	if (class_->events().size())
	{
		stream << endl << "// ----- Events: -----" << endl;
	}

	for (auto event : class_->events())
	{
		stream << format(event);
	}

	stream << "// }" << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EventRef event) const
{
	stream << "~namespace~" << formatName(event) << " = function() { };" << endl << endl;

	stream << "~namespace~" << formatName(event) << ".prototype" << " = Object.create(" <<
		"evb.Event.prototype);" << endl << endl;

	stream << "~namespace~" << formatName(event) << ".TYPE_ID =  " << " " <<
		boost::lexical_cast<std::string>(event->typeId()) << ";" << endl << endl << endl;

		for (auto value : event->values()) {
			stream << "Object.defineProperty(" << "~namespace~" << formatName(event) << ".prototype, '" << formatName(value) <<
				"', {get: function() { /*impl*/ }, set: function(new" << value->longName() <<
				") { /*impl*/ }}); /*" << format(value->type()) << "*/" << endl << endl;
		}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	stream << endl << "var " << formatName(namespace_) << " = " << formatName(namespace_) << " || {  }" << endl<< endl;
	
	for ( auto member : namespace_->members() )
	{
        std::size_t count = 0;
		filter(stream).push<counter>(count) << format(member) << (count > 0 ? "\n\n" : "") << flush;
	}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
	stream << "// enum " << formatName(enum_) << endl << "// {" << endl << endl;

	if (enum_->doc())
	{
		stream << format(enum_->doc());
	}

	stream << "~namespace~" << formatName(enum_) << "= {  };" << endl << endl;

	for (auto value : enum_->values())
	{
		if (value->doc())
		{
			stream << format(value->doc());
		}
		stream << "~namespace~" << formatName(enum_) << "." << formatName(value) << " = " << format(value) << "," << endl << endl;
	} 

	stream << "// }" << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ValueRef value) const
{
	stream << value->value();
}

void JSHeaderFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
	stream << formatSig(operation) << ";" << endl << endl;
}

void JSHeaderFormatter::formatSig(std::ostream& stream, Model::OperationRef operation) const
{
	if(operation->doc())
	{
		stream << format(operation->doc());
	}

	if(operation->result())
	{
		if (!operation->isSynchronous())
		{
			stream << "/*Promise[" << format(operation->result()->type()) << " " << formatName(operation->result()) << "]*/";
		}
		else 
		{
			stream << format(operation->result()->type()) <<  " " << formatName(operation->result());
		}
	}
	else 
	{
		if (!operation->isSynchronous())
		{
			stream << "/*Promise[ ]*/";
		}
		else
		{
			stream << "/*void*/";
		}
	}
	

	string prototype;
	if (operation->isStatic())
	{
		prototype = "prototype.";
	}
	else
	{
		prototype = "";
	}

    stream << " " << _langConfigReader.formatNamespace(operation) << prototype <<  formatName(operation) <<  " = function" << "(";

	for( auto parameter : indices(operation->params()) )
	{
		stream << format(parameter.value()) << (!parameter.last() ? ", " : "");
	}

	stream << ")";
}

void JSHeaderFormatter::format(std::ostream& stream, Model::NamespaceMemberRef member) const
{
	if (auto doc = member->doc())
	{
		stream << format(doc);
	}

	if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(member))
	{
		stream << format(primitive);
	}
	else
	if (auto container = std::dynamic_pointer_cast<Model::Container>(member))
	{
		stream << format(container);
	}
	else
	if (auto constant = std::dynamic_pointer_cast<Model::Constant>(member))
	{
		stream << format(constant);
	}
	else
	if (auto struct_ = std::dynamic_pointer_cast<Model::Struct>(member))
	{
		stream << format(struct_);
	}
	else
	if (auto class_ = std::dynamic_pointer_cast<Model::Class>(member))
	{
		stream << format(class_);
	}
	else
	if (auto event = std::dynamic_pointer_cast<Model::Event>(member))
	{
		stream << format(event);
	}
	else
	if (auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(member))
	{
		stream << format(namespace_);
	}
	else
	if (auto enum_ = std::dynamic_pointer_cast<Model::Enum>(member))
	{
		stream << format(enum_);
	}
	else
	if (auto operation = std::dynamic_pointer_cast<Model::Operation>(member))
	{
		stream << format(operation);
	}
	else
		throw std::runtime_error("unknown namespace member type");
}


} } } // namespace: Everbase::InterfaceCompiler::Components
