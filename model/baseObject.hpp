#ifndef OBJECT_H
#define OBJECT_H

#include "string"
#include "boost/shared_ptr.hpp"
#include "model/documentation.hpp"

class Object
{
public:
    Object();

    std::string name();
    void setName(std::string name);

    std::string shortName();
    void setShortName(std::string name);

    std::shared_ptr<Object> parent();
    void setParent(std::shared_ptr<Object> parent);

    DocumentationPtr doc();
    void setDoc(DocumentationPtr doc);

private:
    std::string mName;
    std::string mShortName;
    std::shared_ptr<Object> mParent;
    DocumentationPtr mDoc;
};

#endif // OBJECT_H
