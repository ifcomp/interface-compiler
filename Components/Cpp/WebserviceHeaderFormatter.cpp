#include "Components/Cpp/WebserviceHeaderFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void WebserviceHeaderFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
    if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        stream << "namespace " << name(namespace_) << endl << "{" << endl;

        for ( auto element : namespace_->elements() )
        {
            filter(stream).push<indent>(config.indentData) << forwards(element);
        }

        stream << "}" << endl << endl;
    }
    else
    if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        stream << "class " << name(class_) << "Impl;" << endl;
    }
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

    stream << "class " << name(class_) << "Impl : public virtual " << name(class_) << endl << "{" << endl;

    if(class_->operations().size() > 0)
    {
        stream << "public:" << endl;

        for( auto operation : class_->operations() )
        {
            if( !operation->isStatic() )
            {
                filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
            }
        }
    }

    stream << "public:" << endl;

    filter(stream).push<indent>(config.indentData)
        << name(class_) << "Impl();" << endl
        << name(class_) << "Impl(const " << name(class_) << "Impl& other) = delete;" << endl
        << name(class_) << "Impl(" << name(class_) << "Impl&& other) = delete;" << endl
        << "virtual ~" << name(class_) << "Impl();" << endl << endl;

    filter(stream).push<indent>(config.indentData)
        << name(class_) << "Impl& operator=(const " << name(class_) << "Impl& other) = delete;" << endl
        << name(class_) << "Impl& operator=(" << name(class_) << "Impl&& other) = delete;" << endl << endl;

    stream << "};" << endl;
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation);

    if( !operation->isStatic() )
    {
        stream << " override";
    }

    stream << ";" << endl;
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void WebserviceHeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
