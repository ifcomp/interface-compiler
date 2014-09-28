#pragma once

#include "Parser.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Components {

class StandardParser : public Parser
{
public:
	virtual Model::RootRef execute ( const ConfigProvider& cprov, std::istream& input ) const override;
};

} } } // namespace: Everbase::InterfaceCompiler::Components
