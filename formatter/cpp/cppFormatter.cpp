#include "formatter/cpp/cppFormatter.hpp"

using namespace Api::Gen;
using namespace Api::Model;
using namespace std;


CppFormatter::CppFormatter(string configFilename)
    : Formatter(configFilename)
{

}

void CppFormatter::param(ostream &stream, ParameterPtr param)
{
    type(stream, param->type(), true);
    stream << " ";
    name(stream, param);
}


void CppFormatter::result(ostream &stream, ParameterPtr param, bool fullyQualified)
{
    if (param)
    {
        type(stream, param->type(), fullyQualified);
    }
    stream << "void";
}


void CppFormatter::operation(ostream &stream, OperationPtr operation)
{
    string delimiter = mParser.configAttribute<string>(Parser::LangConfigParser::StyleAttribute::NAME_DELIMITER, operation);

    IdentifiablePtr parentObject = dynamic_pointer_cast<Identifiable>(operation->parentObject());
    if (parentObject == nullptr)
    {
        throw runtime_error("CppFormatter::operation() : parent object " +
                            operation->parentObject()->objectTypeName() +
                            " is not an Identifiable!\n");
    }

    doc(stream, operation->doc());
    result(stream, operation->result(), true);
    stream << " ";
    name(stream, parentObject);
    stream << delimiter;
    name(stream, operation);
    stream << "(";

    for (auto paramPair : operation->params())
    {
        param(stream, paramPair.second);
        stream << ", ";
    }

    if (operation->params().size())
    {
        // remove trailing delimiter
//        output.resize(output.size() - 2);
    }

    stream << ")";
}


void CppFormatter::event(ostream &stream, EventPtr event)
{
}
