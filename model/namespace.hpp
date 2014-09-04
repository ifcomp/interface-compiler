#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <string>
#include "model/class.hpp"

class Namespace
{
public:
    Namespace();

    std::string name();
    void setName(std::string name);

    ClassPtr classes();
    void addClass(ClassPtr apiClass);

private:
    std::string mName;
    std::vector<ClassPtr> mClasses;
};

#endif // NAMESPACE_H
