#pragma once

#include "Model/Type.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class UnresolvedType : public Type
{
public:
    static const char* TYPE_NAME;

private:
    std::string _primary;
    std::vector<std::string> _params;

public:
    UnresolvedType();
    virtual ~UnresolvedType();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    std::string primary();
    void setPrimary(std::string primary);

    void addParam(std::string param);
    const std::vector<std::string>& params();
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
