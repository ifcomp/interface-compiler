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
	T& get(std::tuple<Types...> tuple)
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

	struct NameConfig
	{
		NameStyle style;
		std::string delimiter;
		bool useShort;

		NameConfig(NameStyle style, std::string delimiter, bool useShort)
			: style(style), delimiter(delimiter), useShort(useShort)
		{ }

		NameConfig(const NameConfig& other)
			: style(other.style), delimiter(other.delimiter), useShort(other.useShort)
		{ }

		NameConfig(NameConfig&& other)
			: style(std::move(other.style))
			, delimiter(std::move(other.delimiter))
			, useShort(std::move(other.useShort))
		{ }
	};

	template<typename T>
	struct TypeNameConfig : public NameConfig
	{
		using NameConfig::NameConfig;
	};

	typedef std::tuple<
		TypeNameConfig<Model::Namespace>,
		TypeNameConfig<Model::Parameter>,
		TypeNameConfig<Model::Enum>,
		TypeNameConfig<Model::Value>,
		TypeNameConfig<Model::Event>,
		TypeNameConfig<Model::Struct>,
		TypeNameConfig<Model::Class>,
		TypeNameConfig<Model::Operation>,
		TypeNameConfig<Model::Constant>
	> NameConfigs;

	std::string namespaceDelimiter;
	std::string indentData;
	std::uint8_t documentationWrapping;
	NameConfigs nameConfigs;

	FormatterConfig(std::string namespaceDelimiter, std::string indentData, std::uint8_t documentationWrapping, NameConfigs nameConfigs)
		: namespaceDelimiter(namespaceDelimiter)
		, indentData(indentData)
		, documentationWrapping(documentationWrapping)
		, nameConfigs(nameConfigs)
	{ }

	FormatterConfig(const FormatterConfig& other)
		: namespaceDelimiter(other.namespaceDelimiter)
		, indentData(other.indentData)
		, documentationWrapping(other.documentationWrapping)
		, nameConfigs(other.nameConfigs)
	{ }

	FormatterConfig(FormatterConfig&& other)
		: namespaceDelimiter(std::move(other.namespaceDelimiter))
		, indentData(std::move(other.indentData))
		, documentationWrapping(std::move(other.documentationWrapping))
		, nameConfigs(std::move(other.nameConfigs))
	{ }

	template<typename T>
	TypeNameConfig<T> nameConfig() const
	{
		return TupleHelper::get<TypeNameConfig<T>>(nameConfigs);
	}
};

} } // namespace: Everbase::InterfaceCompiler
