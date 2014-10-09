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

    const TypeBaseRef &super() const;
    void setSuper(const TypeBaseRef &super);

    void addOperation(const OperationRef &operation);
    std::vector<OperationRef> operations() const;

    void addEvent(const EventRef &event);
    std::vector<EventRef> events() const;

    void addConstant(const ConstantRef &constant);
    std::vector<ConstantRef> constants() const;

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
    //Behavior _behavior;
    TypeBaseRef _super;
    std::vector<OperationRef> _operations;
    std::vector<EventRef> _events;
    std::vector<ConstantRef> _constants;
};

} } } // namespace Everbase::InterfaceCompiler::Model
