#pragma once

#include "Model/TypeBase.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class UnresolvedType : public TypeBase
{
public:
    static const char* TYPE_NAME;
    virtual std::string objectTypeName() override { return TYPE_NAME; }

private:
    std::string _primary;
    std::vector<std::string> _params;

public:
    UnresolvedType();
    virtual ~UnresolvedType();

    std::string primary();
    void setPrimary(std::string primary);

    void addParam(std::string param);
    const std::vector<std::string>& params();
};

typedef std::shared_ptr<UnresolvedType> UnresolvedTypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model
