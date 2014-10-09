#pragma once

#include "Model/Object.hpp"

#include <string>
#include <memory>
#include <vector>
#include <list>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Documentation : public Object
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
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

public:
    Documentation();
    virtual ~Documentation();
    virtual ObjectRef clone() const override;

public:
    bool keyExists(std::string doxygenKey) const;
    std::vector<DocEntry> docEntries() const;
    std::vector<DocEntry> docEntries(std::string doxygenKey) const;
    DocEntry docEntry(std::string doxygenKey) const;
    std::string description(std::string doxygenKey) const;

    void addDocEntry(const DocEntry &entry);
    void addDocEntry(std::string doxygenKey, std::string description);
    void addDocEntry(std::string doxygenKey, std::string paramName, std::string description);

protected:
    virtual void clone(const ObjectRef &clonedObject) const override;

private:
    std::vector<DocEntry> _docEntries;
};

typedef std::shared_ptr<Documentation> DocumentationRef;

} } } // namespace Everbase::InterfaceCompiler::Model
