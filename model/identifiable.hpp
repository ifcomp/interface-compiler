#ifndef IDENTIFIABLE_HPP
#define IDENTIFIABLE_HPP

#include <string>
#include <memory>
#include "model/documentation.hpp"

namespace Api { namespace Model {

class Identifiable;
typedef std::shared_ptr<Identifiable> IdentifiablePtr;

class Identifiable
{
public:
    Identifiable(IdentifiablePtr parentIdentifiable = nullptr);
    virtual ~Identifiable();

    IdentifiablePtr parentIdentifiable();
    void setParentIdentifiable(IdentifiablePtr parentIdentifiable);

    std::string longName();
    void setLongName(std::string longName);

    std::string shortName();
    void setShortName(std::string shortName);

    DocumentationPtr doc();
    void setDoc(DocumentationPtr doc);

private:
    IdentifiablePtr mParentIdentifiable;
    std::string mLongName;
    std::string mShortName;
    DocumentationPtr mDoc;
};


} } // namespace Api::Model

#endif // IDENTIFIABLE_HPP
