#pragma once

#include "Model/DomainObject.hpp"
#include "Model/Documentation.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Identifiable : public DomainObject
{
public:
    Identifiable(DomainObjectRef parentObject = nullptr);
    virtual ~Identifiable();

    std::string longName();
    void setLongName(std::string longName);

    std::string shortName();
    void setShortName(std::string shortName);

    DocumentationRef doc();
    void setDoc(DocumentationRef doc);

private:
    std::string mLongName;
    std::string mShortName;
    DocumentationRef mDoc;
};

typedef std::shared_ptr<Identifiable> IdentifiableRef;

} } } // namespace Everbase::InterfaceCompiler::Model
