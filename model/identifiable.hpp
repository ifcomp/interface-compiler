#ifndef IDENTIFIABLE_HPP
#define IDENTIFIABLE_HPP

#include <string>
#include <memory>
#include "model/domainObject.hpp"
#include "model/documentation.hpp"

namespace Api { namespace Model {

class Identifiable : public DomainObject
{
public:
    Identifiable(DomainObjectPtr parentObject = nullptr);
    virtual ~Identifiable();

    std::string longName();
    void setLongName(std::string longName);

    std::string shortName();
    void setShortName(std::string shortName);

    DocumentationPtr doc();
    void setDoc(DocumentationPtr doc);

private:
    std::string mLongName;
    std::string mShortName;
    DocumentationPtr mDoc;
};

typedef std::shared_ptr<Identifiable> IdentifiablePtr;

} } // namespace Api::Model

#endif // IDENTIFIABLE_HPP
