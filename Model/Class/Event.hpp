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
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Event();
    virtual ~Event();
    virtual ObjectRef clone() const override;

public:
    void addValue(const ParameterRef &value);
    const std::vector<ParameterRef> &values() const;

    void setTypeId(boost::uuids::uuid typeId);
    boost::uuids::uuid typeId() const;

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
    std::vector<ParameterRef> _values;
    boost::uuids::uuid _typeId;
};

} } } // namespace Everbase::InterfaceCompiler::Model
