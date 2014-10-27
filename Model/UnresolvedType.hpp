#pragma once

#include "Model/TypeBase.hpp"

#include <vector>
#include <string>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class UnresolvedType : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    UnresolvedType();
    virtual ~UnresolvedType();
    virtual ObjectRef clone() const override;

public:
    std::string primary() const;
    void setPrimary(std::string primary);

    void addParam(std::string param);
    const std::vector<std::string>& params() const;

protected:
    void clone(const ObjectRef &clonedObject) const override;

private:
    std::string _primary;
    std::vector<std::string> _params;
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
