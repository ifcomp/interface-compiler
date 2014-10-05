#pragma once

#include "Model/Element.hpp"
#include "Model/Type.hpp"

#include <memory>
#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    class Constant;
    class Event;
    class Operation;

    typedef std::shared_ptr<Constant> ConstantRef;
    typedef std::shared_ptr<Event> EventRef;
    typedef std::shared_ptr<Operation> OperationRef;

public:
    enum class Behavior
    {
        VALUE,
        INTERFACE
    };

public:
    Class();
    virtual ~Class();

public:
    Behavior behavior();
    void setBehavior(Behavior behavior);

    TypeBaseRef super();
    void setSuper(TypeBaseRef super);

    void addOperation(OperationRef operation);
    std::vector<OperationRef> operations();

    void addEvent(EventRef event);
    std::vector<EventRef> events();

    void addConstant(ConstantRef constant);
    std::vector<ConstantRef> constants();

private:
    Behavior _behavior;
    TypeBaseRef _super;
    std::vector<OperationRef> _operations;
    std::vector<EventRef> _events;
    std::vector<ConstantRef> _constants;
};

typedef std::shared_ptr<Class> ClassRef;

} } } // namespace Everbase::InterfaceCompiler::Model
