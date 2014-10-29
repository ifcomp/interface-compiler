#include "Components/ObjectiveC/WrapperFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace ObjectiveC {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void WrapperFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include \"everbase/everbase.h\"" << endl
           << endl;
}

void WrapperFormatter::_forwards(std::ostream& stream, Model::ElementRef element) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// " << qname(class_) << ": {" << endl << endl;

    stream << "template<>" << endl
           << "struct TypeEncoding<" << cpp.qname(class_) << "Ref>" << endl
           << "{" << endl
           << "    inline static " << cpp.qname(class_) << "Ref decode(" << qname(class_) << "* src)" << endl
           << "    {" << endl
           << "        if(!src || ![src data])" << endl
           << "        {" << endl
           << "            return " << cpp.qname(class_) << "Ref();" << endl
           << "        }" << endl
           << "        return *(static_cast<" << cpp.qname(class_) << "Ref*>([src data]));" << endl
           << "    }" << endl
           << endl
           << "    inline static " << qname(class_) << " decode(" << cpp.qname(class_) << "Ref src)" << endl
           << "    {" << endl
           << "        if(!src)" << endl
           << "            { return nil; }" << endl
           << "        return *(static_cast<" << cpp.qname(class_) << "Ref*>([src data]));" << endl
           << "    }" << endl
           << "};" << endl
           << endl << endl;

    stream << "@implementation " << qname(class_) << endl;

    if(class_->operations().size() > 0)
    {
        for( auto operation : class_->operations() )
        {
            stream << definition(operation) << endl;
        }
    }

    if(class_->super())
    {
        stream
            << "- (id) init {" << endl
            << "    return [super init];" << endl
            << "}" << endl
            << endl;
    }
    else
    {
        stream
            << "- (id) init {" << endl
            << "    self = [super init];" << endl
            << "    if(self) {" << endl
            << "        self->_data = new " << cpp.qname(class_) << "Ref();" << endl
            << "    }" << endl
            << "    return self;" << endl
            << "}" << endl
            << endl;
    }

    stream
        << "- (id) initWithData:(void*)data {" << endl
        << "    self = [self init];" << endl
        << "    if(self) {" << endl
        << "        *(static_cast<std::shared_ptr<" << cpp.qname(class_) << "Ref>*>(self->_data)) = *(static_cast<std::shared_ptr<" << cpp.qname(class_) << "Ref>*>(data));" << endl
        << "    }" << endl
        << "    return self;" << endl
        << "}" << endl
        << endl
        << "- (void) dealloc {" << endl
        << "    if(self->_data != 0) {" << endl
        << "        delete static_cast<std::shared_ptr<" << cpp.qname(class_) << "Ref>*>(self->_data);" << endl
        << "        self->_data = 0;" << endl
        << "    }" << endl
        << "    [super dealloc];" << endl
        << "}" << endl
        << endl
        << "- (void*) data {" << endl
        << "    return self->_data;" << endl
        << "}" << endl
        << endl;

    stream << "@end" << endl;

    for( auto constant : class_->constants() )
    {
        stream << definition(constant) << endl;
    }

    if(class_->events().size() > 0)
    {
        for( auto event : class_->events() )
        {
            stream << definition(event) << endl;
        }
    }

    stream << "// " << qname(class_) << ": }" << endl << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    auto class_ = std::dynamic_pointer_cast<Class>(constant->parent());

    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "const " << type(constant->type()) << " " << qname(class_) << name(constant) << " = ";

    if( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
                stream << "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
                stream << (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
                stream << "@\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "[[NSUUID alloc] initWithUUIDBytes:(uuid_t){ ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " }]";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

    stream << ";" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Class>(event->parent());

    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "const NSString* " << qname(class_) << name(event) << "TypeName = @\"" << qcname(event) << "\";" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("parent is not a class");

    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation);

    stream << " {" << endl
           << "    using namespace everbase::internal::library::objc;" << endl
           << "    try {" << endl;

    if(operation->result())
    {
        stream << "        return TypeEncoding<" << cpp.type(operation->result()->type()) << ">::encode(";
    }
    else
    {
        stream << "    ";
    }

    if(operation->isStatic())
    {
        stream << cpp.qname(operation);
    }
    else
    {
        stream << "TypeEncoding<" << cpp.qname(class_) << "Ref>::decode(self)->" << cpp.name(operation);
    }

    stream << "(";

    for(auto param : indices(operation->params()))
    {
        stream << "TypeEncoding<" << cpp.type(param.value()->type()) << ">::decode(" << name(param.value()) << ")";

        if(!param.last())
            { stream << ", "; }
    }

    stream << ")";

    if(operation->result())
    {
        stream << ");" << endl;
    }
    else
    {
        stream << ";" << endl;
    }

    stream << "    }" << endl
           << "    catch(const std::exception& e) {" << endl
           << "        @throw TypeEncoding<std::exception>::encode(e);" << endl
           << "    }" << endl
           << "    catch(...) {" << endl
           << "        @throw [NSException exceptionWithName:@\"Unknown Exception\" reason:@\"Unknown exception within C++ context\" userInfo:nil];" << endl
           << "    }" << endl
           << "}" << endl;
}

void WrapperFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
}

void WrapperFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::ObjectiveC
