#ifndef CLASS_HPP
#define CLASS_HPP

#include <memory>
#include "model/namespace_member.hpp"
#include "model/operation.hpp"
#include "model/event.hpp"

namespace Api { namespace Model {

class Class : public NamespaceMember
{
public:
    Class();
    virtual ~Class();

    enum class ClassType
    {
        VALUE,
        ABSTRACT
    };

    ClassType type();
    void setType(ClassType type);

    std::shared_ptr<Class> parent();
    void setParent(std::shared_ptr<Class> parent);

    void addOperation(OperationPtr operation);
    const std::map<std::string, OperationPtr>& operations();

    void addEvent(EventPtr event);
    const std::map<std::string, EventPtr>& events();

private:
    ClassType mType;
    std::shared_ptr<Class> mParent;
    std::map<std::string, OperationPtr> mOperations;
    std::map<std::string, EventPtr> mEvents;
};

typedef std::shared_ptr<Class> ClassPtr;

} } // namespace Api::Model

#endif // CLASS_HPP
