#pragma once

#include "Model/Parameter.hpp"

#include <string>
#include <map>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Event : public Identifiable
{
public:
    static constexpr auto TYPE_NAME = "Event";

private:
    bool _isStatic;
    std::map<std::string, ParameterRef> _results;

public:
    Event();
    virtual ~Event();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool isStatic();
    void setStatic(bool isStatic);

    void addResult(ParameterRef result);
    const std::map<std::string, ParameterRef>& results();
};

typedef std::shared_ptr<Event> EventRef;

} } } // namespace Everbase::InterfaceCompiler::Model
