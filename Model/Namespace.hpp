#pragma once

#include "Model/Element.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Namespace : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() override { return TYPE_NAME; }

public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();

public:
    void addElement(ElementRef element);
    std::vector<ElementRef> elements();

private:
    std::vector<ElementRef> _elements;
};

typedef std::shared_ptr<Namespace> NamespaceRef;

} } } // namespace Everbase::InterfaceCompiler::Model
