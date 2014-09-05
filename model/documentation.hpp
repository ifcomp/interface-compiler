#ifndef DOCUMENTATION_HPP
#define DOCUMENTATION_HPP

#include <string>
#include <memory>

namespace Api { namespace Model {

class Documentation
{
public:
    Documentation();
    virtual ~Documentation();

    std::string brief();
    void setBrief(std::string brief);

    std::string more();
    void setMore(std::string more);

private:
    std::string mBrief;
    std::string mMore;
};

typedef std::shared_ptr<Documentation> DocumentationPtr;

} } // namespace Api::Model

#endif // DOCUMENTATION_HPP
