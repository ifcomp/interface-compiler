#include "Components/TestFormatter.hpp"

#include <set>

namespace Everbase { namespace InterfaceCompiler { namespace Components {

void TestFormatter::formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
	stream << "Name";
}

void TestFormatter::format(std::ostream& stream, Model::RootRef root) const
{
	stream << format(Model::ClassRef()) << std::endl;
}

void TestFormatter::format(std::ostream& stream, Model::TypeRef type) const
{
	stream << "std::shared_ptr<Type>";
}

void TestFormatter::format(std::ostream& stream, Model::ClassRef class_) const
{
	stream << "class " << formatName(Model::IdentifiableRef()) << std::endl << "{" << std::endl;

	for( auto o : std::set<Model::OperationRef>{ Model::OperationRef(), Model::OperationRef(), Model::OperationRef() } )
	{
		stream << format(o) << std::endl;
	}

	stream << "};" << std::endl;
}

void TestFormatter::format(std::ostream& stream, Model::OperationRef operation) const
{
	stream << formatSig(operation) << ";" << std::endl;
}

void TestFormatter::formatSig(std::ostream& stream, Model::OperationRef operation) const
{
	stream << "void test(int a, float b)";
}

} } } // namespace: Everbase::InterfaceCompiler::Components
