#pragma once

#include "Model/Type.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class UnresolvedType : public Type
{
public:
    static constexpr auto TYPE_NAME = "UnresolvedType";

private:
    std::string mPrimary;
    std::vector<std::string> mParams;

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
