#include "formatter.hpp"

//using namespace Api::Gen;

Api::Gen::Formatter::Formatter(std::string typemapFilename, std::string configFilename)
{
}

std::string Api::Gen::Formatter::name(Api::Model::IdentifiablePtr identifiable)
{
    return "";
}

std::string Api::Gen::Formatter::type(Api::Model::TypePtr type, bool fullyQualified)
{
    return "";
}

std::string Api::Gen::Formatter::classType(Api::Model::ClassPtr classPtr)
{
    return "";
}

std::string Api::Gen::Formatter::param(Api::Model::ParameterPtr param)
{
    return "";
}

std::string Api::Gen::Formatter::doc(Api::Model::Documentation doc)
{
    return "";
}
