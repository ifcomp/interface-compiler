#include "Components/TypeScript/WebClientFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <typeinfo>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace TypeScript {

using std::string;

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;


void WebClientFormatter::_definition(std::ostream& stream, Model::NamespaceRef namespace_) const
{
    if (!namespace_->parent()->parent() && namespace_->longName() == "Everbase")
    {
        //stream << "\'use strict\';" << endl << endl;
    }

    if ( namespace_->doc() )
    {
        stream << doc(namespace_->doc());
    }

    if (!namespace_->parent()->parent())
    {
        //stream << "var " << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
        if (namespace_->longName() == "Everbase")
        {
            stream << "everbase.rpc = everbase.rpc || { }" << endl << endl;
            stream << "everbase.rpc.webSocket = everbase.rpc.webSocket || { }" << endl << endl;
        }
    }
    else
    {
        stream << qname(namespace_) << " = " << qname(namespace_) << " || { };" << endl << endl;
    }

    for ( auto element : namespace_->elements() )
    {
        filter(stream) << definition(element);
    }
}


void WebClientFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }

    stream << "// struct: " << qname(struct_) << " {" << endl << endl;
    stream << qname(struct_) << " = function() { };" << endl << endl;

    for (auto field : struct_->fields())
    {
        if ( field->doc() )
        {
            stream << doc(field->doc());
        }

        stream << "Object.defineProperty(" << qname(struct_) << ".prototype, '" << name(field) << "', "
               << "{ get: function() { return this._" << name(field) << " }, set: function(" << name(field) << ") { this._" << name(field) << " = " << name(field) << " } } ); "
               << "/* " << type(field->type()) << " */" << endl << endl;
    }

    stream << "// struct: }" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }

    stream << "// class: " << qname(class_) << " {" << endl << endl;

    stream << qname(class_) << " = function(handle) {" << endl
        << "    this._handle = typeof handle !== 'undefined' ? handle : null;" << endl
        << "    this._destroyed = false;" << endl
        << "};" << endl << endl;

    if( auto super = std::dynamic_pointer_cast<Model::Type>(class_->super()) )
    {
        if (auto superClass = std::dynamic_pointer_cast<Model::Class>(super->primary()))
        {
            stream << qname(class_) << ".prototype" << " = " << "Object.create(" << qname(superClass) << ".prototype);" << endl << endl;
        }
    }

    for (auto constant : class_->constants())
    {
        stream << definition(constant);
    }

    for (auto operation : class_->operations())
    {
        stream << definition(operation);
    }

    // destroy() has a custom body
    auto destroy = std::make_shared<Model::Class::Operation>();
    destroy->setLongName("destroy");
    destroy->setParent(class_);

    stream << signature(destroy) << " {" << endl
        << "    if(this._destroyed) {" << endl
        << "        throw new Error(\"Tried to call function on object after destroy() was called.\");" << endl
        << "    }" << endl;

    filter f(stream);
    f << endl;
    f.push<indent>( config.indentData ) << "var message: any =" << endl;
    f << "[" <<endl;
    f.push<indent>( config.indentData )
        << "\'call\'," << endl
        << "\'Everbase::Internal::Kernel::WebService::ProxyObjectDestroyed\'," << endl
        << "uuid()," << endl
        << "["; !destroy->isStatic() || destroy->params().size() ? f << endl : f;
    f.push<indent>( config.indentData );
    f << "this._handle," << endl;
    f.pop() << "]" << endl;
    f.pop() << "];";
    f << endl;
    f << "everbase.webclient.connection.send(message);" << endl;
    stream << endl;
    f << "this._destroyed = true;" << endl;
    f << "delete everbase.rpc.jsonEncoding._handles[this._handle]" << endl;
    f << "return new Promise(function (resolve, reject) {" << endl;
    f.push<indent>( config.indentData );
    f << "everbase.webclient.processes[message[2]] = [ resolve, reject, '', [] ];" << endl;
    f.pop()
        << "});" << endl;
    f.pop()
        << "};" << endl << endl;

    for (auto event : class_->events())
    {
        stream << definition(event);
    }

    stream << "// class: }" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << qname(constant) << " = ";

    if ( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
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
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::uuids::to_string(boost::any_cast<boost::uuids::uuid>(constant->value()));
                    uuid.erase(std::remove(uuid.begin(), uuid.end(), '-'), uuid.end());
                    stream << "\"" << uuid << "\"";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }
    stream << ";" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }

    stream << "// event: " << qname(event) << " {" << endl << endl;

    stream << qname(event) << " = function() { };" << endl << endl;
    stream << qname(event) << ".prototype" << " = Object.create(everbase.Event.prototype);" << endl << endl;
    stream << qname(event) << ".TYPE_NAME =" << endl;
    stream << qname(event) << ".prototype.TYPE_NAME = '" << qcname(event) << "'" << endl << endl;

    for (auto value : event->values())
    {
        if ( value->doc() )
        {
            stream << doc(value->doc());
        }

        stream << "Object.defineProperty(" << qname(event) << ".prototype, '" << name(value)
            << "', { get: function() { return this._" << name(value) << " }, set: function(" << name(value)
            << ") { this._" << name(value) << " = " << name(value) << " } } ); "
            << "/* " << type(value->type()) << " */" << endl << endl;
    }

    stream << "// event: }" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if(operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation) << " {" << endl;

    stream << "    if(this._destroyed) {" << endl
        << "        throw new Error(\"Tried to call function on object after destroy() was called.\");" << endl
        << "    }" << endl;

    _formatRequest(stream, operation);

    filter f(stream);
    f.push<indent>( config.indentData ) << "everbase.webclient.connection.send(message);" << endl;
    stream << endl;
    f << "return new Promise(function (resolve, reject) {" << endl;

    f.push<indent>( config.indentData );

    string formattedParams = "";
    if (auto result = operation->result())
    {
        f << "everbase.webclient.processes[message[2]] = [ resolve, reject, '";
        _returnType(f, result);
        f << "' , [";
        _containerTypes(f, result);
        f << "] ];" << endl;
    }
    else
    {
        f << "everbase.webclient.processes[message[2]] = [ resolve, reject, '', [] ];" << endl;
    }

    f.pop()
        << "});" << endl;

    f.pop()
        << "};" << endl << endl;
}


void WebClientFormatter::_formatRequest(std::ostream& stream, Model::Class::OperationRef operation) const
{
    string paramTypes = "";

    filter f(stream);
    f << endl;
    f.push<indent>( config.indentData ) << "var message: any =" << endl;
    f << "[" <<endl;
    f.push<indent>( config.indentData )
        << "\'call\'," << endl
        << "\'" << qcname(operation) << "\'," << endl
        << "uuid()," << endl
        << "["; !operation->isStatic() || operation->params().size() ? f << endl : f;
    f.push<indent>( config.indentData );
    if (!operation->isStatic()) { f << "this._handle," << endl; };
    for (auto param : operation->params())
    {
        f << "everbase.rpc.jsonEncoding._conversions['";
        _returnType(f, param);
        f << "'].encode( " << name(param) << ", [";
        _containerTypes(f, param);
        f << "] )," << endl;
    }
    f.pop() << "]" << endl;
    f.pop() << "];";
    f << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }

    stream << "// enum: " << qname(enum_) << " {" << endl << endl;

    stream << qname(enum_) << " = { };" << endl << endl;

    for (auto value : enum_->values())
    {
        stream << definition(value);
    }

    stream << "// enum: }" << endl << endl;
}


void WebClientFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    if (value->doc())
    {
        stream << doc(value->doc());
    }

    stream << qname(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value()) << ";" << endl << endl;
}

void WebClientFormatter::_returnType(filter& f, Model::ParameterRef param) const
{
    auto paramType = std::dynamic_pointer_cast<Model::Type>(param->type())->primary();
    //if primitive
    if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(paramType))
    {
        f << primitive->underlyingName();
    }
    else
    {
        f << qcname(paramType);
    }
}


void WebClientFormatter::_containerTypes(filter& f, Model::ParameterRef containerParam) const
{
    bool isFirst = false;
    //if has container-params
    if (std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params().size())
    {
        auto typeParams = std::dynamic_pointer_cast<Model::Type>(containerParam->type())->params();
        for (auto typeParam : indices(typeParams))
        {
            //if primitive
            if (auto primitive = std::dynamic_pointer_cast<Model::Primitive>(typeParam.value()))
            {
                isFirst = typeParam.first();
                isFirst ? f << "'" << primitive->underlyingName() << "'" : f << ", '" << primitive->underlyingName() << "'";
            }
            else
            {
                typeParam.first() ? f << "'" << qcname(typeParam.value()) << "'" : f << ", '" << qcname(typeParam.value()) << "'";
            }
        }
    }
}


} } } } // namespace: Everbase::InterfaceCompiler::Components::TypeScript
