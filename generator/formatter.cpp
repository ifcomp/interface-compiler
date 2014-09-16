#include "formatter.hpp"

using namespace Api::Gen;
using namespace std;

Formatter::Formatter(std::string configFilename)
{
    cout << "starting to parse configfile " << configFilename << endl;
    mYamlConfig = YAML::LoadFile(configFilename);

    for (auto sequenceNode : mYamlConfig["typemap"])
    {
        cout << "a node with type " << sequenceNode.Type() << endl;


//    if (sequenceNode.IsMap())
//    {
//        for (YAML::const_iterator it = sequenceNode.begin(); it != sequenceNode.end(); ++it)
//        {
//            const YAML::Node &key = it->first;
//            const YAML::Node &value = it->second;

//            if (key.IsScalar())
//            {
//                cout << key.Scalar() << ": ";
//                if (value.IsScalar())
//                {
//                    cout << value.Scalar();
//                }
//                else
//                {
//                    cout << " <value type = " << value.Type() << ">";
//                }
//                cout << endl;
//            }
//            else
//            {
//                cout << "*** KEY TYPE: " << key.Type() << endl;
//            }
//        }
//    }

//        for (auto type : sequenceNode["typemap"])
//        {
//            cout << "typemapping: " << type.first.Scalar() << " => " << type.second.Scalar() << endl;
//        }
    }
}

std::string Formatter::name(Api::Model::IdentifiablePtr identifiable)
{
    return "";
}

std::string Formatter::type(Api::Model::TypePtr type, bool fullyQualified)
{
    return "";
}

std::string Formatter::classType(Api::Model::ClassPtr classPtr)
{
    return "";
}

std::string Formatter::param(Api::Model::ParameterPtr param)
{
    return "";
}

std::string Formatter::doc(Api::Model::Documentation doc)
{
    return "";
}
