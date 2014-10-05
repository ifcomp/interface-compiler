#pragma once

#include "Model/Element.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Namespace : public Element
{
public:
    static const char* TYPE_NAME;

private:
    std::vector<ElementRef> _elements;

public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    void addElement(ElementRef element);
    std::vector<ElementRef> elements();
};

typedef std::shared_ptr<Namespace> NamespaceRef;

} } } // namespace Everbase::InterfaceCompiler::Model
