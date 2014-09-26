#ifndef CLASS_HPP
#define CLASS_HPP

#include <memory>
#include "model/namespace_member.hpp"
#include "model/operation.hpp"
#include "model/event.hpp"
#include "model/type.hpp"
#include <string>

namespace Api { namespace Model {

class Class : public NamespaceMember
{
public:
    static constexpr auto TYPE_NAME = "class";

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

    TypePtr parent();
    void setParent(TypePtr parent);

    void addOperation(OperationPtr operation);
    const std::map<std::string, OperationPtr>& operations();

    void addEvent(EventPtr event);
    const std::map<std::string, EventPtr>& events();

private:
    ClassType mType;
    TypePtr mParent;
    std::map<std::string, OperationPtr> mOperations;
    std::map<std::string, EventPtr> mEvents;
};

typedef std::shared_ptr<Class> ClassPtr;

} } // namespace Api::Model

#endif // CLASS_HPP
