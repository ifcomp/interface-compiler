#include "Components/Ruby/ClassesFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void ClassesFormatter::_includes(std::ostream& stream) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    Model::NamespaceRef parent;

    if(namespace_->parent())
    {
        parent = std::dynamic_pointer_cast<Model::Namespace>(namespace_->parent());
    }

    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc()) << endl;
    }

    stream << "// namespace " << qname(namespace_) << ": {" << endl << endl;

    for ( auto element : namespace_->elements() )
    {
        stream << definition(element);
    }

    stream << "// namespace " << qname(namespace_) << ": }" << endl << endl;
}

void ClassesFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(class_->parent());

    if(!namespace_)
        throw std::runtime_error("invalid class");

    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// class " << qname(class_) << ": {" << endl << endl;

    if(class_->super())
    {
        if( auto super = std::dynamic_pointer_cast<Model::Class>(std::dynamic_pointer_cast<Type>(class_->super())->primary()) )
        {
            stream << "EVERBASE_RUBY_CLASS_WITH_SUPER(" << qcname(class_, "_") << ", " << qcname(namespace_, "_") << ", \"" << name(class_) << "\", " << qcname(super, "_") << ")" << endl << endl;
        }
        else
            throw std::runtime_error("invalid super type");
    }
    else
    {
        stream << "EVERBASE_RUBY_CLASS(" << qcname(class_, "_") << ", " << qcname(namespace_, "_") << ", \"" << name(class_) << "\")" << endl << endl;
    }

    stream << "template<>" << endl
           << "struct TypeEncoding<" << cpp.qname(class_) << "Ref>" << endl
           << "{" << endl
           << "    using unencoded_type = " << cpp.qname(class_) << "Ref;" << endl
           << "    using encoded_type = VALUE;" << endl
           << endl
           << "    inline static unencoded_type decode(encoded_type src)" << endl
           << "    {" << endl
           << "        if(src == Qnil)" << endl
           << "        {" << endl
           << "            return unencoded_type();" << endl
           << "        }" << endl
           << "        unencoded_type* unencoded = nullptr;" << endl
           << "        Data_Get_Struct(src, unencoded_type, unencoded);" << endl
           << "        if(!unencoded)" << endl
           << "        {" << endl
           << "            return unencoded_type();" << endl
           << "        }" << endl
           << "        return *unencoded;" << endl
           << "    }" << endl
           << endl
           << "    inline static encoded_type encode(unencoded_type src)" << endl
           << "    {" << endl
           << "        if(!src)" << endl
           << "            { return Qnil; }" << endl
           << "        return Data_Wrap_Struct(" << qcname(class_, "_") << ", 0, &FreeObject<" << cpp.qname(class_) << "Ref>::free, new unencoded_type(src));" << endl
           << "    }" << endl
           << "};" << endl << endl;

    stream << "// class " << qname(class_) << ": }" << endl << endl;
}

void ClassesFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void ClassesFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
