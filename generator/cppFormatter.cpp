#include "cppFormatter.hpp"

using namespace Api::Gen;
using namespace std;

CppFormatter::CppFormatter(std::string configFilename)
    : Formatter(configFilename)
{

}

string CppFormatter::param(Api::Model::ParameterPtr param)
{
    return type(param->type()) + " " + param->longName();
}
