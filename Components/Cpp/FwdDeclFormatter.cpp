#include "Components/Cpp/FwdDeclFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void FwdDeclFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }
    
    stream << "struct " << name(struct_) << ";" << endl;
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }
    
    stream << "class " << name(class_) << ";" << endl;
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    // nothing
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    // nothing
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    // nothing
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }
    
    stream << "enum class " << name(enum_) << ";" << endl;
}

void FwdDeclFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    // nothing
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
