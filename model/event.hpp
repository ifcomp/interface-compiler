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

    void addParam(ParameterPtr param);
    const std::map<std::string, ParameterPtr>& params();

    void addResult(ParameterPtr result);
    const std::map<std::string, ParameterPtr>& results();

private:
    std::map<std::string, ParameterPtr> mParams;
    std::map<std::string, ParameterPtr> mResults;
};

typedef std::shared_ptr<Event> EventPtr;

} } // namespace Api::Model

#endif // EVENT_HPP
