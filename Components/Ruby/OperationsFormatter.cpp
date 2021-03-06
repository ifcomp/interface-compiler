#include "Components/Ruby/OperationsFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Ruby {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void OperationsFormatter::_includes(std::ostream& stream) const
{
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

    stream << "extern \"C\" VALUE " << qcname(operation, "_") << "_cpp(";

    stream << "VALUE self";

    for( auto param : operation->params() )
    {
        stream << ", VALUE " << name(param);
    }

    stream << ", Exception& exception)" << endl
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
        stream << "        return Qnil;" << endl;
    }

    stream << "    }" << endl
           << "    catch(const std::exception& e)" << endl
           << "    {" << endl
           << "        exception.caught = true;" << endl
           << "        snprintf(exception.message, sizeof(exception.message), \"%s\", e.what());" << endl
           << "    }" << endl
           << "    catch(...)" << endl
           << "    {" << endl
           << "        exception.caught = true;" << endl
           << "        snprintf(exception.message, sizeof(exception.message), \"%s\", \"unknown exception\");" << endl
           << "    }" << endl;

    stream << "    return Qnil;" << endl
           << "}" << endl << endl;

    stream << "extern \"C\" VALUE " << qcname(operation, "_") << "(";

    stream << "VALUE self";

    for( auto param : operation->params() )
    {
        stream << ", VALUE " << name(param);
    }

    stream << ")" << endl
           << "{" << endl
           << "    Exception exception { false, { 0 } };" << endl
           << endl
           << "    VALUE result = " << qcname(operation, "_") << "_cpp(self, ";

    for( auto param : operation->params() )
    {
        stream << name(param) << ", ";
    }

    stream << "exception);" << endl
           << endl
           << "    if(exception.caught)" << endl
           << "    {" << endl
           << "        rb_raise(rb_eException, \"%s\", exception.message);" << endl
           << "    }" << endl
           << endl
           << "    return result;" << endl
           << "}" << endl << endl;

    if(operation->isStatic())
    {
        stream << "EVERBASE_RUBY_OPERATION_STATIC(" << qcname(operation, "_") << ", " << qcname(class_, "_") << ", \"" << name(operation) << "\", " << operation->params().size() << ")" << endl << endl;
    }
    else
    {
        stream << "EVERBASE_RUBY_OPERATION(" << qcname(operation, "_") << ", " << qcname(class_, "_") << ", \"" << name(operation) << "\", " << operation->params().size() << ")" << endl << endl;
    }

    stream << "// operation " << qname(operation) << ": }" << endl << endl;

}

void OperationsFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void OperationsFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Ruby
