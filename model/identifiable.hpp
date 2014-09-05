#ifndef IDENTIFIABLE_HPP
#define IDENTIFIABLE_HPP

#include <string>
#include <memory>
#include "model/name.hpp"
#include "model/documentation.hpp"

namespace Api { namespace Model {

class Identifiable
{
public:
    Identifiable();
    virtual ~Identifiable();

    Name name();
    void setName(Name name);

    DocumentationPtr doc();
    void setDoc(DocumentationPtr doc);

private:
    Name mName;
    DocumentationPtr mDoc;
};

typedef std::shared_ptr<Identifiable> IdentifiablePtr;

} } // namespace Api::Model

#endif // IDENTIFIABLE_HPP
