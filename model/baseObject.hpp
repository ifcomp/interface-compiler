#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "model/documentation.hpp"

class BaseObject
{
public:
    BaseObject();

    std::string name();
    void setName(std::string name);

    std::string shortName();
    void setShortName(std::string name);

    DocumentationPtr doc();
    void setDoc(DocumentationPtr doc);

private:
    std::string mName;
    std::string mShortName;
    DocumentationPtr mDoc;
};

typedef std::shared_ptr<BaseObject> BaseObjectPtr;

#endif // OBJECT_H
