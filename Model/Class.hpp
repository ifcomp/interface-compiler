#pragma once

#include "Model/Element.hpp"
#include "Model/Type.hpp"

#include <memory>
#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class;

typedef std::shared_ptr<Class> ClassRef;

class Class : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    class Constant;
    class Event;
    class Operation;

    typedef std::shared_ptr<Constant> ConstantRef;
    typedef std::shared_ptr<Event> EventRef;
    typedef std::shared_ptr<Operation> OperationRef;

public:
    //enum class Behavior
    //{
    //    VALUE,
    //    INTERFACE
    //};

public:
    Class();
    virtual ~Class();
    virtual ObjectRef clone() const override;

public:
    //Behavior behavior() const;
    //void setBehavior(Behavior behavior);

    TypeBaseRef super() const;
    void setSuper(TypeBaseRef super);

    void addOperation(OperationRef operation);
    std::vector<OperationRef> operations() const;

    void addEvent(EventRef event);
    std::vector<EventRef> events() const;

    void addConstant(ConstantRef constant);
    std::vector<ConstantRef> constants() const;

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    //Behavior _behavior;
    TypeBaseRef _super;
    std::vector<OperationRef> _operations;
    std::vector<EventRef> _events;
    std::vector<ConstantRef> _constants;
};

} } } // namespace Everbase::InterfaceCompiler::Model
