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
    Event();
    virtual ~Event();

    bool isStatic();
    void setStatic(bool isStatic);

    void addResult(ParameterPtr result);
    const std::map<std::string, ParameterPtr>& results();

private:
    bool mIsStatic;
    std::map<std::string, ParameterPtr> mResults;
};

typedef std::shared_ptr<Event> EventPtr;

} } // namespace Api::Model

#endif // EVENT_HPP
