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
    static constexpr auto TYPE_NAME     = "Documentation";

    static constexpr auto KEY_BRIEF     = "brief";
    static constexpr auto KEY_MORE      = "more";
    static constexpr auto KEY_PARAM     = "param";
    static constexpr auto KEY_RETURN    = "return";

    struct DocEntry
    {
        std::string doxygenKey;
        std::string paramName;
        std::string description;
    };

private:
    std::vector<DocEntry> _DocEntries;

public:
    Documentation();
    virtual ~Documentation();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    bool keyExists(std::string doxygenKey);
    const std::vector<DocEntry> &docEntries();
    std::vector<DocEntry> docEntries(std::string doxygenKey);
    DocEntry docEntry(std::string doxygenKey);
    std::string description(std::string doxygenKey);

    void addDocEntry(DocEntry entry);
    void addDocEntry(std::string doxygenKey, std::string description);
    void addDocEntry(std::string doxygenKey, std::string paramName, std::string description);
};

typedef std::shared_ptr<Documentation> DocumentationRef;

} } } // namespace Everbase::InterfaceCompiler::Model
