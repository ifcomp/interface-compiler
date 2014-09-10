#include "unresolvedType.hpp"
#include <iostream>

using namespace Api::Model;
using namespace std;

UnresolvedType::UnresolvedType()
{
}

Api::Model::UnresolvedType::~UnresolvedType()
{
    cout << "CLEANUP of UnresolvedType " << mPrimary << endl;
}

std::string Api::Model::UnresolvedType::primary()
{
    return mPrimary;
}

void Api::Model::UnresolvedType::setPrimary(std::string typeName)
{
    mPrimary = typeName;
}

void UnresolvedType::addParam(std::string param)
{
    mParams.push_back(param);
}

const std::vector<std::string> &UnresolvedType::params()
{
    return mParams;
}
