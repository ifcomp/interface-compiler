#pragma once

#include "Model/Model.hpp"

#include <tuple>

namespace Everbase { namespace InterfaceCompiler {

namespace TupleHelper
{
	template<int Index, class Search, class First, class... Types>
	struct get_internal
	{
	    typedef typename get_internal<Index + 1, Search, Types...>::type type;
	        static constexpr int index = Index;
	};

	template<int Index, class Search, class... Types>
	struct get_internal<Index, Search, Search, Types...>
	{
	    typedef get_internal type;
	    static constexpr int index = Index;
	};

	template<class T, class... Types>
	T get(std::tuple<Types...> tuple)
	{
	    return std::get<get_internal<0,T,Types...>::type::index>(tuple);
	}
}

struct FormatterConfig
{
    enum class NameStyle
    {
        LOWER_CAMELCASE,
        UPPER_CAMELCASE,
        LOWERCASE,
        UPPERCASE
    };

	template<typename T>
	struct NameConfig
	{
		NameStyle style;
		std::string delimiter;
		bool useShort;
	};

	typedef std::tuple<
		NameConfig<Model::Namespace>,
		NameConfig<Model::Parameter>,
		NameConfig<Model::Enum>,
		NameConfig<Model::Value>,
		NameConfig<Model::Event>,
		NameConfig<Model::Struct>,
		NameConfig<Model::Class>,
		NameConfig<Model::Operation>,
		NameConfig<Model::Constant>
	> Naming;

	std::string indent;
	std::uint8_t docWrap;
	Naming naming;

	FormatterConfig(std::string indent, std::uint8_t docWrap, Naming naming)
		: indent(indent)
		, docWrap(docWrap)
		, naming(naming)
	{ }

	FormatterConfig(const FormatterConfig& other)
		: indent(other.indent)
		, docWrap(other.docWrap)
		, naming(other.naming)
	{ }

	FormatterConfig(FormatterConfig&& other)
		: indent(std::move(other.indent))
		, docWrap(std::move(other.docWrap))
		, naming(std::move(other.naming))
	{ }

	template<typename T>
	const NameConfig<T>& nameConfig()
	{
		TupleHelper::get<T>(naming);
	}
};

} } // namespace: Everbase::InterfaceCompiler
