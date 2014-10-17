#include "Components/Cpp/RpcJsonFormatter.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void RpcJsonFormatter::_includes(std::ostream& stream) const
{
    FormatterBase::_includes(stream);

    stream << "#include <json_spirit/json_spirit.h>" << endl
           << "#include \"common/rpc/json/TypeEncoding.hpp\"" << endl
           << "#include \"common/rpc/json/OperationEncoding.hpp\"" << endl
           << "#include \"common/rpc/json/EventEncoding.hpp\"" << endl
           << endl;
}

void RpcJsonFormatter::_footer(std::ostream& stream, Model::RootRef root) const
{
    stream << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::json::OperationEncoding>> everbase::internal::common::rpc::json::OperationEncoding::operations {" << endl
           << backwards(root->getNamespace())
           << "};" << endl;

    stream << endl;
    stream << "const std::map<std::string, std::shared_ptr<everbase::internal::common::rpc::json::EventEncoding>> everbase::internal::common::rpc::json::EventEncoding::events {" << endl
           << definition(root->getNamespace(), 0)
           << "};" << endl;
}

void RpcJsonFormatter::_backwards(std::ostream& stream, Model::ElementRef element) const
{
    if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        for ( auto element : namespace_->elements() )
        {
            filter(stream).push<indent>(config.indentData) << backwards(element);
        }
    }
    else
    if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        for( auto operation : class_->operations() )
        {
            stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::json::OperationEncoding>>{\"" << qcname(operation) << "\", std::shared_ptr<everbase::internal::common::rpc::json::OperationEncoding>(new "
                   << "everbase::internal::common::rpc::json::" << qcname(operation, "_") << "())}," << endl;
        }
    }
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::ElementRef element, std::uint8_t pass) const
{
    if(pass == 0)
    {
        if( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
        {
            for ( auto element : namespace_->elements() )
            {
                filter(stream).push<indent>(config.indentData) << definition(element, pass);
            }
        }
        else
        if( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
        {
            for( auto event : class_->events() )
            {
                stream << "std::pair<std::string, std::shared_ptr<everbase::internal::common::rpc::json::EventEncoding>>{\"" << qcname(event) << "\", std::shared_ptr<everbase::internal::common::rpc::json::EventEncoding>(new "
                       << "everbase::internal::common::rpc::json::" << qcname(event, "_") << "())}," << endl;
            }
        }
    }
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
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

void RpcJsonFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc()) << endl;
    }

    stream << "// struct " << qname(struct_) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace json {" << endl << endl;

    stream
        << "template<>" << endl
        << "struct TypeEncoding<" << qname(struct_) << ">" << endl
        << "{" << endl
        << "    static inline json_spirit::mValue encode(everbase::internal::common::rpc::ObjectDirectory& directory, " << qname(struct_) << " source)" << endl
        << "    {" << endl
        << "        json_spirit::mObject object;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        object[\"" << cname(field) << "\"] = TypeEncoding<" << type(field->type()) << ">::encode(directory, source." << name(field) << ");" << endl;
    }

    stream
        << endl
        << "        return json_spirit::mValue(object);" << endl
        << "    }" << endl
        << endl;

    stream
        << "    static inline " << qname(struct_) << " decode(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        const json_spirit::mObject& object = source.get_obj();" << endl
        << "        " << qname(struct_) << " struct_;" << endl << endl;

    for( auto field : struct_->fields() )
    {
        stream
            << "        struct_." << name(field) << " = TypeEncoding<" << type(field->type()) << ">::decode(directory, object.at(\"" << cname(field) << "\"));" << endl;
    }

    stream
        << endl
        << "        return struct_;" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::json" << endl << endl;

    stream << "// struct " << name(struct_) << ": }" << endl << endl;
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc()) << endl;
    }

    stream << "// class " << qname(class_) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace json {" << endl << endl;

    stream
        << "template<>" << endl
        << "struct TypeEncoding<" << qname(class_) << "Ref>" << endl
        << "{" << endl
        << "    static inline json_spirit::mValue encode(everbase::internal::common::rpc::ObjectDirectory& directory, " << qname(class_) << "Ref source)" << endl
        << "    {" << endl
        << "        return json_spirit::mValue(directory.template registerObject<" << qname(class_) << ">(source));" << endl
        << "    }" << endl
        << endl
        << "    static inline " << qname(class_) << "Ref decode(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        return directory.lookupObject<" << qname(class_) << ">(source.get_uint64());" << endl
        << "    }" << endl
        << "};" << endl << endl;


    for( auto operation : class_->operations() )
    {
        stream << definition(operation);
    }

    for( auto event : class_->events() )
    {
        stream << definition(event);
    }

    stream << "} } } } } // namespace: everbase::internal::common::rpc::json" << endl << endl;

    stream << "// class " << name(class_) << ": }" << endl << endl;
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(event->parent());

    if(!class_)
        throw std::runtime_error("invalid event");

    stream
        << "struct " << qcname(event, "_") <<  " : public everbase::internal::common::rpc::json::EventEncoding" << endl
        << "{" << endl;


    // *** encodeValues

    stream
        << "    virtual inline json_spirit::mValue encodeValues(everbase::internal::common::rpc::ObjectDirectory& directory, std::vector<boost::any> values) const override" << endl
        << "    {" << endl
        << "        json_spirit::mArray encoded;" << endl;

    std::size_t i = 0;

    for( auto value : event->values() )
    {
        stream
            << "        encoded.push_back(everbase::internal::common::rpc::json::TypeEncoding<" << type(value->type()) << ">::encode(directory, boost::any_cast<" << type(value->type()) << ">(values[" << i << "])));" << endl;
        i += 1;
    }

    stream
        << "        return encoded;" << endl
        << "    }" << endl
        << endl;


    // *** decodeValues

    stream
        << "    virtual inline std::vector<boost::any> decodeValues(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue values) const override" << endl
        << "    {" << endl;

    if(event->values().size() > 0)
    {
        stream
            << "        const json_spirit::mArray& encoded = values.get_array();" << endl
            << "        std::vector<boost::any> decoded;" << endl;

        i = 0;

        for( auto value : event->values() )
        {
            stream
                << "        decoded.push_back(boost::any(everbase::internal::common::rpc::json::TypeEncoding<" << type(value->type()) << ">::decode(directory, encoded[" << i << "])));" << endl;
            i += 1;
        }

        stream
            << "        return decoded;" << endl;
    }
    else
    {
        stream
            << "        return std::vector<boost::any>();" << endl;
    }

    stream
        << "    }" << endl
        << endl;

    stream
        << "};" << endl << endl;
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    auto class_ = std::dynamic_pointer_cast<Model::Class>(operation->parent());

    if(!class_)
        throw std::runtime_error("invalid operation");

    stream
        << "struct " << qcname(operation, "_") <<  " : public everbase::internal::common::rpc::json::OperationEncoding" << endl
        << "{" << endl;


    // *** encodeParameters

    stream
        << "    virtual inline json_spirit::mValue encodeParameters(everbase::internal::common::rpc::ObjectDirectory& directory, std::vector<boost::any> parameters) const override" << endl
        << "    {" << endl
        << "        json_spirit::mArray encoded;" << endl;

    if(!operation->isStatic())
    {
        stream
            << "        encoded.push_back(everbase::internal::common::rpc::json::TypeEncoding<" << qname(class_) << "Ref>::encode(directory, boost::any_cast<" << qname(class_) << "Ref>(parameters[0])));" << endl;
    }

    std::size_t i = operation->isStatic() ? 0 : 1;

    for( auto param : operation->params() )
    {
        stream
            << "        encoded.push_back(everbase::internal::common::rpc::json::TypeEncoding<" << type(param->type()) << ">::encode(directory, boost::any_cast<" << type(param->type()) << ">(parameters[" << i << "])));" << endl;
        i += 1;
    }

    stream
        << "        return encoded;" << endl
        << "    }" << endl
        << endl;


    // *** decodeParameters

    stream
        << "    virtual inline std::vector<boost::any> decodeParameters(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue parameters) const override" << endl
        << "    {" << endl;

    if(!operation->isStatic() || operation->params().size() > 0)
    {
        stream
            << "        const json_spirit::mArray& encoded = parameters.get_array();" << endl
            << "        std::vector<boost::any> decoded;" << endl;

        if(!operation->isStatic())
        {
            stream
                << "        decoded.push_back(boost::any(everbase::internal::common::rpc::json::TypeEncoding<" << qname(class_) << "Ref>::decode(directory, encoded[0])));" << endl;
        }

        i = operation->isStatic() ? 0 : 1;

        for( auto param : operation->params() )
        {
            stream
                << "        decoded.push_back(boost::any(everbase::internal::common::rpc::json::TypeEncoding<" << type(param->type()) << ">::decode(directory, encoded[" << i << "])));" << endl;
            i += 1;
        }

        stream
            << "        return decoded;" << endl;
    }
    else
    {
        stream
            << "        return std::vector<boost::any>();" << endl;
    }

    stream
        << "    }" << endl
        << endl;


    // *** encodeResult

    stream
        << "    virtual inline json_spirit::mValue encodeResult(everbase::internal::common::rpc::ObjectDirectory& directory, boost::any result) const override" << endl
        << "    {" << endl;

    if(operation->result())
    {
        stream
            << "        return everbase::internal::common::rpc::json::TypeEncoding<" << type(operation->result()->type()) << ">::encode(directory, boost::any_cast<" << type(operation->result()->type()) << ">(result));" << endl;   
    }
    else
    {
        stream
            << "        return json_spirit::mValue();" << endl; 
    }

    stream
        << "    }" << endl << endl;


    // *** decodeResult

    stream
        << "    virtual inline boost::any decodeResult(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue result) const override" << endl
        << "    {" << endl;

    if(operation->result())
    {
        stream
            << "        return everbase::internal::common::rpc::json::TypeEncoding<" << type(operation->result()->type()) << ">::decode(directory, result);" << endl;   
    }
    else
    {
        stream
            << "        return boost::any();" << endl; 
    }

    stream
        << "    }" << endl;

    stream
        << "};" << endl << endl;
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc()) << endl;
    }

    stream << "// enum " << qname(enum_) << ": {" << endl << endl;

    stream << "namespace everbase { namespace internal { namespace common { namespace rpc { namespace json {" << endl << endl;

    stream
        << "template<>" << endl
        << "struct TypeEncoding<" << qname(enum_) << ">" << endl
        << "{" << endl
        << "    static inline json_spirit::mValue encode(everbase::internal::common::rpc::ObjectDirectory& directory, " << qname(enum_) << " source)" << endl
        << "    {" << endl
        << "        return json_spirit::mValue(static_cast<std::uint64_t>(source));" << endl
        << "    }" << endl
        << endl
        << "    static inline " << qname(enum_) << " decode(everbase::internal::common::rpc::ObjectDirectory& directory, json_spirit::mValue source)" << endl
        << "    {" << endl
        << "        return static_cast<" << qname(enum_) << ">(source.get_uint64());" << endl
        << "    }" << endl
        << "};" << endl << endl;

    stream << "} } } } } // namespace: everbase::internal::common::rpc::json" << endl << endl;

    stream << "// enum " << name(enum_) << ": }" << endl << endl;
}

void RpcJsonFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp
