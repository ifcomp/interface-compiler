#include "Model/Identifiable.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

Identifiable::Identifiable()
{
}

Identifiable::~Identifiable()
{
}

std::string Identifiable::longName() const
{
	if(_longName.length() == 0)
		return _shortName;

    return _longName;
}

void Identifiable::setLongName(std::string longName)
{
    _longName = longName;
}

std::string Identifiable::shortName() const
{
	if(_shortName.length() == 0)
		return _longName;

    return _shortName;
}

void Identifiable::setShortName(std::string shortName)
{
    _shortName = shortName;
}

const DocumentationRef &Identifiable::doc() const
{
	return _doc;
}

void Identifiable::setDoc(const DocumentationRef &doc)
{
    doc->setParent(shared_from_this());
    _doc = doc;
}

void Identifiable::clone(const ObjectRef &clonedObject) const
{
    using namespace std;

    IdentifiableRef clonedIdentifiable = dynamic_pointer_cast<Identifiable>(clonedObject);

    if (clonedIdentifiable)
    {
        clonedIdentifiable->setLongName(longName());
        clonedIdentifiable->setShortName(shortName());

        if (doc())
        {
            clonedIdentifiable->setDoc(dynamic_pointer_cast<Documentation>(doc()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Identifiable - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
