#include "Model/Documentation.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Documentation::TYPE_NAME     = "Documentation";

const char* Documentation::KEY_BRIEF     = "brief";
const char* Documentation::KEY_MORE      = "more";
const char* Documentation::KEY_PARAM     = "param";
const char* Documentation::KEY_RETURN    = "return";

Documentation::Documentation()
{
}


Documentation::~Documentation()
{
}

ObjectRef Documentation::clone() const
{
    DocumentationRef newDoc = std::make_shared<Documentation>();
    clone(newDoc);
    return newDoc;
}

bool Documentation::keyExists(std::string doxygenKey) const
{
    return _docEntries.find( doxygenKey ) != _docEntries.end();
}

std::vector<Documentation::DocEntry> Documentation::docEntries() const
{
    std::vector<DocEntry> entries;
    entries.reserve( _docEntries.size() );

    for( auto& entry : _docEntries )
    {
        entries.push_back( entry.second );
    }

    return entries;
}

std::vector<Documentation::DocEntry> Documentation::docEntries(std::string doxygenKey) const
{
    std::vector<DocEntry> entries;

    auto range = _docEntries.equal_range( doxygenKey );
    if( range.first != range.second )
    {
        entries.reserve( std::distance( range.first, range.second ) );

        for( auto it = range.first; it != range.second; ++range.first )
        {
            entries.push_back( it->second );
        }
    }

    return entries;
}

Documentation::DocEntry Documentation::docEntry(std::string doxygenKey) const
{
    auto it = _docEntries.find( doxygenKey );
    if( it != _docEntries.end() )
    {
        return it->second;
    }
    throw std::runtime_error("doxygenKey " + doxygenKey + " not set");
}

std::string Documentation::description(std::string doxygenKey) const
{
    try {
        return docEntry(doxygenKey).description;
    }
    catch (const std::runtime_error &e)
    {
        throw;
    }
}

void Documentation::addDocEntry(const DocEntry &entry)
{
    _docEntries.insert( std::pair<std::string, DocEntry>( entry.doxygenKey, entry ) );
}

void Documentation::addDocEntry(std::string doxygenKey, std::string description)
{
    _docEntries.insert( std::pair<std::string, DocEntry>( doxygenKey, DocEntry{ doxygenKey, "", description } ) );
}

void Documentation::addDocEntry(std::string doxygenKey, std::string paramName, std::string description)
{
    _docEntries.insert( std::pair<std::string, DocEntry>( doxygenKey, DocEntry{ doxygenKey, paramName, description } ) );
}

void Documentation::clone(const ObjectRef &clonedObject) const
{
    using namespace std;

    DocumentationRef clonedDoc = dynamic_pointer_cast<Documentation>(clonedObject);

    if (clonedDoc)
    {
        for (auto docEntry : docEntries())
        {
            clonedDoc->addDocEntry(docEntry);
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Documentation - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
