#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace std;

int main()
{
    try {
        YAML::Node config = YAML::LoadFile("objects.yaml");

        for (std::size_t n = 0; n < config.size(); ++n)
        {
            if (config[n])
            {
                cout << config[n].Scalar() << config[n].Type() << endl;
            }
            else
            {
                cout << "undefined node" << endl;
            }
        }

    }
    catch (YAML::Exception e)
    {
        cout << "OOPS: " << e.what() << endl;
    }

    cout << "done" << endl;
    return 0;
}

