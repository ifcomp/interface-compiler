#pragma once

#include "Model/Model.hpp"
#include "ConfigProvider.hpp"
#include "FormatToken.hpp"
#include "StreamFilter.hpp"

#include <memory>
#include <ostream>

namespace Everbase { namespace InterfaceCompiler {

class Formatter
{
public:
	void execute ( const ConfigProvider& cprov, Model::RootRef input, std::ostream& output ) const;

protected:
	FormatToken<Model::IdentifiableRef> formatName(Model::IdentifiableRef identifiable) const;
	FormatToken<Model::RootRef> format(Model::RootRef root) const;
	FormatToken<Model::TypeRef> format(Model::TypeRef type) const;
	FormatToken<Model::ClassRef> format(Model::ClassRef class_) const;
	FormatToken<Model::OperationRef> format(Model::OperationRef operation) const;
	FormatToken<Model::OperationRef> formatSig(Model::OperationRef operation) const;

protected:
	virtual void formatName(std::ostream& stream, Model::IdentifiableRef identifiable) const = 0;
	virtual void format(std::ostream& stream, Model::RootRef root) const = 0;
	virtual void format(std::ostream& stream, Model::TypeRef type) const = 0;
	virtual void format(std::ostream& stream, Model::ClassRef class_) const = 0;
	virtual void format(std::ostream& stream, Model::OperationRef operation) const = 0;
	virtual void formatSig(std::ostream& stream, Model::OperationRef operation) const = 0;
};

} } // namespace: Everbase::InterfaceCompiler
