#pragma once

#include "Model/NamespaceMember.hpp"
#include "Model/Operation.hpp"
#include "Model/Event.hpp"
#include "Model/Type.hpp"

#include <memory>
#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class : public NamespaceMember
{
public:
    static const char* TYPE_NAME;

    enum class ClassType
    {
        VALUE,
        INTERFACE
    };

public:
    Class();
    virtual ~Class();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    ClassType type();
    void setType(ClassType type);

    TypeBaseRef parent();
    void setParent(TypeBaseRef parent);

    void addOperation(OperationRef operation);
    std::vector<OperationRef> operations();

    void addEvent(EventRef event);
    std::vector<EventRef> events();

private:
    ClassType _type;
    TypeBaseRef _parent;
    std::vector<OperationRef> _operations;
    std::vector<EventRef> _events;
};

typedef std::shared_ptr<Class> ClassRef;

} } } // namespace Everbase::InterfaceCompiler::Model
