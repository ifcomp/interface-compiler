#include "Components/TestFormatter.hpp"

#include <set>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

using std::endl;
using IndexList::indices;
using namespace Model;
using namespace StreamFilter;

TestFormatter::TestFormatter(std::istream &configStream)
    : _langConfigReader(configStream)
{
    _langConfigReader.parseTypeMap();
}

void TestFormatter::format(std::ostream& stream, Model::RootRef root) const
{
	stream << format(root->getNamespace()) << endl;
}

void TestFormatter::formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    stream << _langConfigReader.styleToken(identifiable->longName(), identifiable);
//    stream << identifiable->longName();
}

void TestFormatter::format(std::ostream& stream, Model::DocumentationRef documentation) const
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

void TestFormatter::format(std::ostream& stream, Model::TypeRef type) const
{
	stream << formatName(type->primary());
}

void TestFormatter::format(std::ostream& stream, Model::ParameterRef parameter) const
{
	stream << format(parameter->type()) << " " << formatName(parameter);
}

void TestFormatter::format(std::ostream& stream, Model::NamespaceMemberRef member) const
{
	if( auto doc = member->doc() )
	{
		stream << format(doc);
	}

	if( auto primitive = std::dynamic_pointer_cast<Model::Primitive>(member) )
	{
		stream << format(primitive);
	}
	else
	if( auto container = std::dynamic_pointer_cast<Model::Container>(member) )
	{
		stream << format(container);
	}
	else
	if( auto constant = std::dynamic_pointer_cast<Model::Constant>(member) )
	{
		stream << format(constant);
	}
	else
	if( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(member) )
	{
		stream << format(struct_);
	}
	else
	if( auto class_ = std::dynamic_pointer_cast<Model::Class>(member) )
	{
		stream << format(class_);
	}
	else
	if( auto event = std::dynamic_pointer_cast<Model::Event>(member) )
	{
		stream << format(event);
	}
	else
	if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(member) )
	{
		stream << format(namespace_);
	}
	else
	if( auto enum_ = std::dynamic_pointer_cast<Model::Enum>(member) )
	{
		stream << format(enum_);
	}
	else
	if( auto operation = std::dynamic_pointer_cast<Model::Operation>(member) )
	{
		stream << format(operation);
	}
	else
		throw std::runtime_error("unknown namespace member type");
}

void TestFormatter::format(std::ostream& stream, Model::PrimitiveRef primitive) const
{
	stream << "!TODO primitive" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::ContainerRef container) const
{
	stream << "!TODO container" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::ConstantRef constant) const
{
	stream << "!TODO constant" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::StructRef struct_) const
{
	stream << "!TODO struct" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "class " << formatName(class_) << endl << "{" << endl;

	for( auto operation : class_->operations() )
	{
		filter(stream).push<indent>() << format(operation) << endl;
	}

	stream << "}" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::EventRef event) const
{
	stream << "!TODO event" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::NamespaceRef namespace_) const
{
	stream << "namespace " << formatName(namespace_) << endl << "{" << endl;

	for ( auto member : namespace_->members() )
	{
		filter(stream).push<indent>() << format(member) << endl;
	}

	stream << "}" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::EnumRef enum_) const
{
	stream << "!TODO enum" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::ValueRef value) const
{
	stream << "!TODO value" << endl;
}

void TestFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
	stream << formatSig(operation) << ";" << endl;
}

void TestFormatter::formatSig(std::ostream& stream, Model::OperationRef operation) const
{
	if(operation->doc())
	{
		stream << format(operation->doc());
	}

	if(operation->result())
	{
		stream << format(operation->result()->type());
	}
	else
	{
		stream << "void";
	}

	stream << " " << formatName(operation) << "(";

	/*
	for( auto parameter : operation->params() )
	{
		stream << format(parameter) << ", ";
	}
	*/

	for( auto parameter : indices(operation->params()) )
	{
		stream << format(parameter.value()) << (!parameter.last() ? ", " : "");
	}

	stream << ")";
}

} } } // namespace: Everbase::InterfaceCompiler::Components
