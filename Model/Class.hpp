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
    static constexpr auto TYPE_NAME = "Class";

    enum class ClassType
    {
        VALUE,
        ABSTRACT
    };

public:
    Class();
    virtual ~Class();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    ClassType type();
    void setType(ClassType type);

    TypeRef parent();
    void setParent(TypeRef parent);

    void addOperation(OperationRef operation);
    const std::map<std::string, OperationRef>& operations();

    void addEvent(EventRef event);
    const std::map<std::string, EventRef>& events();

private:
    ClassType mType;
    TypeRef mParent;
    std::map<std::string, OperationRef> mOperations;
    std::map<std::string, EventRef> mEvents;
};

typedef std::shared_ptr<Class> ClassRef;

} } } // namespace Everbase::InterfaceCompiler::Model
