#include "Components/Cpp/LibraryHeaderFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void LibraryHeaderFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream
        << "#include \"Everbase/Threading/SafeAccess.hpp\"" << endl
        << "#include \"Everbase/Primitives/EventManager.hpp\"" << endl
        << endl
        << "#include <list>" << endl
        << "#include <boost/system/error_code.hpp>" << endl
        << "#include <boost/system/system_error.hpp>" << endl
        << "#include <boost/uuid/uuid_generators.hpp>" << endl
        << "#include <boost/uuid/uuid_io.hpp>" << endl
        << "#include <boost/date_time/posix_time/posix_time.hpp>" << endl
        << "#include <boost/algorithm/string.hpp>" << endl
        << "#include <boost/asio.hpp>" << endl
        << endl
        << "#ifdef WIN32" << endl
        << "#include <codecvt>" << endl
        << "#ifndef NOMINMAX" << endl
        << "#define NOMINMAX 1" << endl
        << "#endif" << endl
        << "#include <Windows.h>" << endl
        << "#undef NOMINMAX" << endl
        << "#endif" << endl
        << endl
        << "#if defined(__APPLE__) || defined(WIN32)" << endl
        << "#include <boost/thread/tss.hpp>" << endl
        << "#endif" << endl
        << endl;
}

void LibraryHeaderFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
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

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

    stream << "class " << name(class_) << "Impl : public virtual " << name(class_);

    if(class_->super())
    {
        if( auto super = std::dynamic_pointer_cast<Model::Class>(std::dynamic_pointer_cast<Type>(class_->super())->primary()) )
        {
            stream << ", public virtual " << qname(super) << "Impl";
        }
        else
            throw std::runtime_error("invalid super type");
    }

    stream << endl << "{" << endl;

    filter(stream).push<reset>() << "#include \"dummy_impl/" << qname(class_, "/") << "Impl/decl.hpp\"" << endl;

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
//        << name(class_) << "Impl();" << endl
        << name(class_) << "Impl(const " << name(class_) << "Impl& other) = delete;" << endl
        << name(class_) << "Impl(" << name(class_) << "Impl&& other) = delete;" << endl
        << "virtual ~" << name(class_) << "Impl();" << endl << endl;

    filter(stream).push<indent>(config.indentData)
        << name(class_) << "Impl& operator=(const " << name(class_) << "Impl& other) = delete;" << endl
        << name(class_) << "Impl& operator=(" << name(class_) << "Impl&& other) = delete;" << endl << endl;

    stream << "};" << endl;
}

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
}

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
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

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void LibraryHeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
