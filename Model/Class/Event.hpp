#pragma once

#include "Model/Parameter.hpp"
#include "Model/Class.hpp"

#include <string>
#include <memory>
#include <boost/uuid/uuid.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class::Event : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Event();
    virtual ~Event();

public:
    void addValue(ParameterRef value);
    std::vector<ParameterRef> values();

    void setTypeId(boost::uuids::uuid typeId);
    boost::uuids::uuid typeId();

private:
    std::vector<ParameterRef> _values;
    boost::uuids::uuid _typeId;
};

} } } // namespace Everbase::InterfaceCompiler::Model
