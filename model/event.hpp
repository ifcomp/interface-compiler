#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <map>
#include <memory>
#include "model/parameter.hpp"

namespace Api { namespace Model {

class Event : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "event";

private:
    bool mIsStatic;
    std::map<std::string, ParameterPtr> mResults;

public:
    Event();
    virtual ~Event();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool isStatic();
    void setStatic(bool isStatic);

    void addResult(ParameterPtr result);
    const std::map<std::string, ParameterPtr>& results();
};

typedef std::shared_ptr<Event> EventPtr;

} } // namespace Api::Model

#endif // EVENT_HPP
