#ifndef DOCUMENTATION_HPP
#define DOCUMENTATION_HPP

#include <string>
#include <memory>
#include <vector>
#include <list>
#include "model/domainObject.hpp"

namespace Api { namespace Model {

class Documentation : public DomainObject
{
public:
    static constexpr auto TYPE_NAME = "doc";

private:
    std::string mBrief;
    std::string mMore;
    std::vector<std::list<std::string>> mDocEntries;

public:
    Documentation();
    virtual ~Documentation();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    std::string brief();
    void setBrief(std::string brief);

    std::string more();
    void setMore(std::string more);

    const std::vector<std::list<std::string>> &docEntries();
    void addDocEntry(std::string doxygenKey, std::list<std::string> elements);
    void addDocEntry(std::string doxygenKey, std::string paramName, std::string description);
};

typedef std::shared_ptr<Documentation> DocumentationPtr;

} } // namespace Api::Model

#endif // DOCUMENTATION_HPP
