#pragma once

#include "Model/Identifiable.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Element : public Identifiable
{
public:
    Element();
    virtual ~Element();

    using Identifiable::clone;
};

typedef std::shared_ptr<Element> ElementRef;

} } } // namespace Everbase::InterfaceCompiler::Model
