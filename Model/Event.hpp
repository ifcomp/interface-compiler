#pragma once

#include "Model/Parameter.hpp"

#include <string>
#include <map>
#include <memory>
#include <boost/uuid/uuid.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Event : public Identifiable
{
public:
    static const char* TYPE_NAME;

private:
    std::map<std::string, ParameterRef> _values;
    boost::uuids::uuid _typeId;

public:
    Event();
    virtual ~Event();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addValue(ParameterRef value);
    std::vector<ParameterRef> values();

    void setTypeId(boost::uuids::uuid typeId);
    boost::uuids::uuid typeId();
};

typedef std::shared_ptr<Event> EventRef;

} } } // namespace Everbase::InterfaceCompiler::Model
