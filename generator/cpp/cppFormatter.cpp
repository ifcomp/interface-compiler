#include "generator/cpp/cppFormatter.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace std;


CppFormatter::CppFormatter(string configFilename)
    : Formatter(configFilename)
{

}

string CppFormatter::param(Api::Model::ParameterPtr param)
{
    return type(param->type(), true) + " " + param->longName();
}


string CppFormatter::result(ParameterPtr param, bool fullyQualified)
{
    if (param)
    {
        return type(param->type(), fullyQualified);
    }
    return "void";
}


string CppFormatter::operation(Api::Model::OperationPtr operation)
{
    string delimiter = "::";    ///< @todo: fetch delimiter from config
    string output;

    output += doc(operation->doc());
    output += indent() + result(operation->result(), true) + " ";
    output += name(operation->parentIdentifiable()) + delimiter + name(operation) + "(";

    for (auto paramPair : operation->params())
    {
        output += param(paramPair.second) + ", ";
    }

    if (operation->params().size())
    {
        // remove trailing delimiter
        output.resize(output.size() - 2);
    }

    output += ");";

    return output;
}


string CppFormatter::event(EventPtr event)
{
    return "";
}
