#include "Components/Ruby/OperationsFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void OperationsFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);
}

void OperationsFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
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

void OperationsFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void OperationsFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    for( auto operation : class_->operations() )
    {
        stream << definition(operation) << endl;
    }
}

void OperationsFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void OperationsFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void OperationsFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("invalid operation");

    if ( operation->doc() )
    {
        stream << doc(operation->doc()) << endl;
    }

    stream << "// operation " << qname(operation) << ": {" << endl << endl;

    stream << "VALUE " << qcname(operation, "_") << "(";

    if( !operation->isStatic() )
    {
        stream << "VALUE self";

        if(operation->params().size() > 0)
            {  stream << ", "; }
    }

    for( auto param : indices(operation->params()) )
    {
        stream << "VALUE " << name(param.value()) << (!param.last() ? ", " : "");
    }

    stream << ")" << endl
           << "{" << endl;

    stream << "    try" << endl
           << "    {" << endl;

    if( operation->result() )
    {
        stream << "        return TypeEncoding<" << cpp.type(operation->result()->type()) << ">::encode(";
    }
    else
    {
        stream << "        ";
    }

    if( !operation->isStatic() )
    {
        stream << "TypeEncoding<" << cpp.qname(class_) << "Ref>::decode(self)->" << cpp.name(operation);
    }
    else
    {
        stream << cpp.qname(operation);
    }

    stream << "(";

    for( auto param : indices(operation->params()) )
    {
        stream << "TypeEncoding<" << cpp.type(param.value()->type()) << ">::decode(" << name(param.value()) << (!param.last() ? "), " : ")");
    }

    if( operation->result() )
    {
        stream << "));" << endl;
    }
    else
    {
        stream << ");" << endl;
    }

    stream << "    }" << endl
           << "    catch(const std::exception& e)" << endl
           << "    {" << endl
           << "        rb_raise(rb_eException, e.c_str());" << endl
           << "    }" << endl
           << "    catch(...)" << endl
           << "    {" << endl
           << "        rb_raise(rb_eException, \"unknown exception\");" << endl
           << "    }" << endl
           << endl;

    stream << "}" << endl;

    stream << "// operation " << qname(operation) << ": }" << endl << endl;

}

void OperationsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void OperationsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
