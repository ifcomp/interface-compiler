#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include <string>
#include <boost/shared_ptr.hpp>

class Documentation
{
public:
    Documentation();

    std::string brief();
    void setBrief(std::string text);

    std::string more();
    void setMore(std::string text);

private:
    std::string mBrief;
    std::string mMore;
};

typedef std::shared_ptr<Documentation> DocumentationPtr;

#endif // DOCUMENTATION_H
