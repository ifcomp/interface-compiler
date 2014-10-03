#pragma once

#include "Model/DomainObject.hpp"

#include <string>
#include <memory>
#include <vector>
#include <list>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Documentation : public DomainObject
{
public:
    static const char* TYPE_NAME;

    static const char* KEY_BRIEF;
    static const char* KEY_MORE;
    static const char* KEY_PARAM;
    static const char* KEY_RETURN;

    struct DocEntry
    {
        std::string doxygenKey;
        std::string paramName;
        std::string description;
    };

private:
    std::vector<DocEntry> _docEntries;

public:
    Documentation();
    virtual ~Documentation();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool keyExists(std::string doxygenKey);
    std::vector<DocEntry> docEntries();
    std::vector<DocEntry> docEntries(std::string doxygenKey);
    DocEntry docEntry(std::string doxygenKey);
    std::string description(std::string doxygenKey);

    void addDocEntry(DocEntry entry);
    void addDocEntry(std::string doxygenKey, std::string description);
    void addDocEntry(std::string doxygenKey, std::string paramName, std::string description);
};

typedef std::shared_ptr<Documentation> DocumentationRef;

} } } // namespace Everbase::InterfaceCompiler::Model
