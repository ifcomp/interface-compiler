#include "Model/Documentation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Documentation::Documentation()
{
}


Documentation::~Documentation()
{
}


bool Documentation::keyExists(std::string doxygenKey)
{
    for (auto entry : _docEntries)
    {
        if (entry.doxygenKey == doxygenKey)
        {
            return true;
        }
    }
    return false;
}


const std::vector<Documentation::DocEntry> &Documentation::docEntries()
{
    return _docEntries;
}


std::vector<Documentation::DocEntry> Documentation::docEntries(std::string doxygenKey)
{
    std::vector<DocEntry> entries;

    for (auto entry : _docEntries)
    {
        if (entry.doxygenKey == doxygenKey)
        {
            entries.push_back(entry);
        }
    }

    return entries;
}


Documentation::DocEntry Documentation::docEntry(std::string doxygenKey)
{
    for (auto entry : _docEntries)
    {
        if (entry.doxygenKey == doxygenKey)
        {
            return entry;
        }
    }
    throw std::runtime_error("doxygenKey " + doxygenKey + " not set");
}


std::string Documentation::description(std::string doxygenKey)
{
    try {
        return docEntry(doxygenKey).description;
    }
    catch (const std::runtime_error &e)
    {
        throw;
    }
}


void Documentation::addDocEntry(DocEntry entry)
{
    _docEntries.push_back(entry);
}


void Documentation::addDocEntry(std::string doxygenKey, std::string description)
{
    _docEntries.push_back(DocEntry{ doxygenKey, "", description });
}


void Documentation::addDocEntry(std::string doxygenKey, std::string paramName, std::string description)
{
    _docEntries.push_back(DocEntry{ doxygenKey, paramName, description });
}

} } } // namespace Everbase::InterfaceCompiler::Model
