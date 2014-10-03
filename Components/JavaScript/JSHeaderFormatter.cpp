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

using NameStyle = FormatterConfig::NameStyle;
template <typename T> using NameConfig = FormatterConfig::TypeNameConfig<T>;
using Naming = FormatterConfig::Naming;

JSHeaderFormatter::JSHeaderFormatter(std::istream &configStream)
    : Formatter(FormatterConfig
        {
            std::string(' ', 4), 85,
            Naming {
                NameConfig<Namespace> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Parameter> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Enum>      { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Value>     { NameStyle::UPPERCASE, "_", false },
                NameConfig<Event>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Struct>    { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class>     { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Operation> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Constant>  { NameStyle::UPPERCASE, "", false }
            }
        })
    , _langConfigReader(configStream)
{
    _langConfigReader.parseTypeMap();
}

void JSHeaderFormatter::format(std::ostream& stream, Model::RootRef root) const
{
	stream << format(root->getNamespace()) << endl;
}

std::string inline JSHeaderFormatter::formatNamespace(Model::IdentifiableRef identifiable) const
{
	return _langConfigReader.formatNamespace(identifiable);
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
    //stream << "~primitive~" << formatName(primitive) << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ContainerRef container) const
{
    //stream << "~container~" << formatName(container) << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ConstantRef constant) const
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
	stream << formatNamespace(constant) << formatName(constant) << ".TYPE_ID = " << 
		valueString << ";" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::StructRef struct_) const
{
	stream << "// struct " << formatName(struct_) << endl << "// {" << endl << endl;
    stream << formatNamespace(struct_) << formatName(struct_) << "= function() {  }" << endl << endl;

	for (auto field : struct_->fields())
	{
		stream << "Object.defineProperty(" << formatNamespace(struct_) << formatName(struct_) << ".prototype, '" << formatName(field) <<
			"', {get: function() { /*impl*/ }, set: function(new" << field->longName() << 
			") { /*impl*/ }}); /*" << format(field->type()) << "*/" << endl << endl;
	}

	stream << "// }" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "// class " << formatName(class_) << endl << "// {" << endl << endl;

	stream << formatNamespace(class_) << formatName(class_) << " = function() { };";

	if( auto parent = std::dynamic_pointer_cast<Model::Type>(class_->parent()) )
	{
		if (auto parentClass = std::dynamic_pointer_cast<Model::Class>(parent->primary()))
		{
			stream << endl << endl << formatNamespace(class_) << "prototype" << "Object.create(" << formatNamespace(parentClass) << formatName(parentClass) << ".prototype);";
		}
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

	stream << "// }" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EventRef event) const
{
	stream << formatNamespace(event) << formatName(event) << " = function() { };" << endl << endl;

	stream << formatNamespace(event) << formatName(event) << ".prototype" << " = Object.create(" <<
		"evb.Event.prototype);" << endl << endl;

	stream << formatNamespace(event) << formatName(event) << ".TYPE_ID = " << endl;
	stream << formatNamespace(event) << formatName(event) << ".prototype.TYPE_ID =" << " \'" <<
		boost::lexical_cast<std::string>(event->typeId()) << "\';" << endl << endl << endl;

		for (auto value : event->values()) {
			stream << "Object.defineProperty(" << formatNamespace(value) << formatName(event) << ".prototype, '" 
				<< formatName(value) <<	"', {get: function() { /*impl*/ }, set: function("
				<< formatName("New" + value->longName(), "New" + value->shortName(), config.nameConfig<Model::Parameter>())
				<< ") { /*impl*/ }}); /*" << format(value->type()) << "*/" << endl << endl;
		}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	stream << endl << "var " << formatNamespace(namespace_) << formatName(namespace_) << " = " 
		<< formatNamespace(namespace_) << formatName(namespace_) << " || {  }" << endl << endl;
	
	for ( auto member : namespace_->members() )
	{
        std::size_t count = 0;
		filter(stream).push<counter>(count) << format(member) << (count > 0 ? "\n\n" : "") << flush;
	}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
	stream << "// enum " << formatName(enum_) << endl << "// {" << endl << endl;

	stream << formatNamespace(enum_) << formatName(enum_) << "= {  };" << endl << endl;

	for (auto value : enum_->values())
	{
		if (value->doc())
		{
			stream << format(value->doc());
		}
		stream << formatNamespace(enum_) << formatName(enum_) << "." << formatName(value) << " = " << format(value) << "," << endl << endl;
	} 

	stream << "// }" << endl << endl;
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
			stream << "/*" << format(operation->result()->type()) <<  " " << formatName(operation->result()) << "*/";
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

    stream << " " << formatNamespace(operation) << prototype <<  formatName(operation) <<  " = function" << "(";

	for( auto parameter : indices(operation->params()) )
	{
		stream << format(parameter.value()) << (!parameter.last() ? ", " : "");
	}

	stream << ")";
}

} } } // namespace: Everbase::InterfaceCompiler::Components
