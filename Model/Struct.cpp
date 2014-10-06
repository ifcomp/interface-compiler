#include "Model/Struct.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Struct::TYPE_NAME = "Struct";

Struct::Struct()
{
}

Struct::~Struct()
{
}

ObjectRef Struct::clone() const
{
    StructRef newStruct = std::make_shared<Struct>();
    clone(newStruct);
    return newStruct;
}

void Struct::addField(ParameterRef field)
{
    field->setParent(shared_from_this());
    _fields.push_back(field);
}

std::vector<ParameterRef> Struct::fields() const
{
    return _fields;
}

void Struct::clone(ObjectRef clonedObject) const
{
    using namespace std;

    StructRef clonedStruct = dynamic_pointer_cast<Struct>(clonedObject);

    if (clonedStruct)
    {
        Identifiable::clone(clonedStruct);

        for (auto field : fields())
        {
            clonedStruct->addField(dynamic_pointer_cast<Parameter>(field->clone()));
        }
    }
    else
    {
        throw runtime_error("clone() failed: expected Struct - got " + clonedObject->typeName());
    }
}

} } } // namespace Everbase::InterfaceCompiler::Model
