#include "Components/JavaScript/JSHeaderFormatter.hpp"

#include <set>
#include <iostream>
#include <sstream>
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
            std::string("."), std::string(4, ' '), 85,
            Naming {
                NameConfig<Namespace> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Parameter> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Enum> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Value> { NameStyle::UPPERCASE, "_", false },
                NameConfig<Event> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Struct> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Class> { NameStyle::UPPER_CAMELCASE, "", false },
                NameConfig<Operation> { NameStyle::LOWER_CAMELCASE, "", false },
                NameConfig<Constant> { NameStyle::UPPERCASE, "", false }
            }
        })
    , _langConfigReader(configStream)
{
    _langConfigReader.parseTypeMap();
}

void JSHeaderFormatter::format(std::ostream& stream, Model::RootRef root) const
{
	stream << format(root->getNamespace());
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
        stream << formatQualifiedName(type->primary());
    }
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
	stream << "/* " << format(parameter->type()) << " */" << " " << formatName(parameter);
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
	stream << formatQualifiedName(constant) << ".TYPE_ID = " << 
		valueString << ";" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::StructRef struct_) const
{
	stream << "// struct: " << formatQualifiedName(struct_) << " {" << endl << endl;
    stream << formatQualifiedName(struct_) << "= function() {  }" << endl << endl;

	for (auto field : struct_->fields())
	{
		stream << "Object.defineProperty(" << formatQualifiedName(struct_) << ".prototype, '" << formatName(field) << "', "
		       << "{get: function() { throw new Error(\"not implemented\"); }, set: function(new" << field->longName() << ") { throw new Error(\"not implemented\"); }}); "
		       << "/* " << format(field->type()) << " */" << endl << endl;
	}

	stream << "// struct: }" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "// class: " << formatQualifiedName(class_) << " {" << endl << endl;

	stream << formatQualifiedName(class_) << " = function() { };" << endl << endl;

	if( auto parent = std::dynamic_pointer_cast<Model::Type>(class_->parent()) )
	{
		if (auto parentClass = std::dynamic_pointer_cast<Model::Class>(parent->primary()))
		{
			stream << formatQualifiedName(class_) << ".prototype" << " = " << "Object.create(" << formatQualifiedName(parentClass) << ".prototype);" << endl << endl;
		}
	}
	else
	{
		stream << formatQualifiedName(class_) << ".prototype" << " = " << "Object.create(Everbase.Observable.prototype);" << endl << endl;
	}

	for (auto operation : class_->operations())
	{
		stream << format(operation);
	}

	for (auto event : class_->events())
	{
		stream << "// event: " << formatQualifiedName(event) << " {" << endl << endl;

		stream << format(event);

		stream << "// event: }" << endl << endl;
	}

	stream << "// class: }" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EventRef event) const
{
	stream << formatQualifiedName(event) << " = function() { };" << endl << endl;

	stream << formatQualifiedName(event) << ".prototype" << " = Object.create(Everbase.Event.prototype);" << endl << endl;

	stream << formatQualifiedName(event) << ".TYPE_ID = " << endl;
	stream << formatQualifiedName(event) << ".prototype.TYPE_ID =" << " \'" << boost::lexical_cast<std::string>(event->typeId()) << "\';" << endl << endl;

	for (auto value : event->values())
	{
		stream << "Object.defineProperty( " << formatQualifiedName(event) << ".prototype, '" 
			<< formatName(value) <<	"', {get: function() { throw new Error(\"not implemented\"); }, set: function("
			<< formatName("New" + value->longName(), "New" + value->shortName(), config.nameConfig<Model::Parameter>())
			<< " ) { throw new Error(\"not implemented\"); }}); /* " << format(value->type()) << " */" << endl << endl;
	}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	stream << "var " << formatQualifiedName(namespace_) << " = " << formatQualifiedName(namespace_) << " || {  }" << endl << endl;
	
	for ( auto member : namespace_->members() )
	{
		filter(stream) << format(member);
	}
}

void JSHeaderFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
	stream << "// enum: " << formatQualifiedName(enum_) << " {" << endl << endl;

	stream << formatQualifiedName(enum_) << "= {  };" << endl << endl;

	for (auto value : enum_->values())
	{
		stream << format(value);
	} 

	stream << "// enum: }" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::ValueRef value) const
{
	if (value->doc())
	{
		stream << format(value->doc());
	}

	stream << formatQualifiedName(std::dynamic_pointer_cast<Model::Identifiable>(value->parentObject())) << "." << formatName(value) << " = " << value->value() << ";" << endl << endl;
}

void JSHeaderFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
	if(operation->doc())
	{
		stream << format(operation->doc());
	}

	stream << formatSig(operation) << " { throw new Error(\"not implemented\"); }" << endl << endl;
}

void JSHeaderFormatter::formatSig(std::ostream& stream, Model::OperationRef operation) const
{
	if(operation->result())
	{
		if (!operation->isSynchronous())
		{
			stream << "/* Promise [" << format(operation->result()->type()) << " " << formatName(operation->result()) << "] */";
		}
		else 
		{
			stream << "/* " << format(operation->result()->type()) <<  " " << formatName(operation->result()) << " */";
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
	
    stream << " " << formatQualifiedName(std::dynamic_pointer_cast<Model::Identifiable>(operation->parentObject())) << (!operation->isStatic() ? ".prototype." : ".") <<  formatName(operation) <<  " = function" << "(";

	for( auto parameter : indices(operation->params()) )
	{
		stream << format(parameter.value()) << (!parameter.last() ? ", " : "");
	}

	stream << ")";
}

} } } // namespace: Everbase::InterfaceCompiler::Components
