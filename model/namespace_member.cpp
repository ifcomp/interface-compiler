#include <iostream>
#include "model/namespace_member.hpp"

using namespace Api::Model;
using namespace std;

NamespaceMember::NamespaceMember()
{
}

NamespaceMember::~NamespaceMember()
{
    cout << "### CLEANUP NamespaceMember " << longName() << endl;
}
