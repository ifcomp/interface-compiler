#pragma once

#include "Model/Element.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Namespace : public Element
{
public:
    static const char* TYPE_NAME;
    virtual std::string typeName() const override { return TYPE_NAME; }

public:
    Namespace();
    Namespace(std::string longName);
    virtual ~Namespace();
    virtual ObjectRef clone() const override;

public:
    void addElement(ElementRef element);
    std::vector<ElementRef> elements() const;

protected:
    void clone(ObjectRef clonedObject) const override;

private:
    std::vector<ElementRef> _elements;

};

typedef std::shared_ptr<Namespace> NamespaceRef;

} } } // namespace Everbase::InterfaceCompiler::Model
