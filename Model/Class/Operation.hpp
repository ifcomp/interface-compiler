#pragma once

#include "Model/Identifiable.hpp"
#include "Model/Parameter.hpp"
#include "Model/Class.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Class::Operation : public Identifiable
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Operation();
    virtual ~Operation();
    virtual ObjectRef clone() const override;

public:
    bool isStatic() const;
    void setStatic(bool isStatic);

    bool isConst() const;
    void setConst(bool isConst);

    //bool isSynchronous() const;
    //void setSynchronous(bool isSynchronous);

    void addParam(const ParameterRef &param);
    std::vector<ParameterRef> params() const;

    void setResult(const ParameterRef &result);
    const ParameterRef &result() const;

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
    bool _isStatic;
    bool _isConst;
    //bool _isSynchronous;
    std::vector<ParameterRef> _params;
    ParameterRef _result;
};

} } } // namespace Everbase::InterfaceCompiler::Model
