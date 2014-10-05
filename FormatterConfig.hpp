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

	struct NameConfigBase
	{
		NameStyle style;
		std::string delimiter;
		bool useShort;

		NameConfigBase(NameStyle style, std::string delimiter, bool useShort)
			: style(style), delimiter(delimiter), useShort(useShort)
		{ }

		NameConfigBase(const NameConfigBase& other)
			: style(other.style), delimiter(other.delimiter), useShort(other.useShort)
		{ }

		NameConfigBase(NameConfigBase&& other)
			: style(std::move(other.style))
			, delimiter(std::move(other.delimiter))
			, useShort(std::move(other.useShort))
		{ }
	};

	template<typename T>
	struct NameConfig : public NameConfigBase
	{
		using NameConfigBase::NameConfigBase;
	};

	typedef std::tuple<
		NameConfig<Model::Namespace>,
		NameConfig<Model::Parameter>,
		NameConfig<Model::Struct>,
		NameConfig<Model::Enum>,
		NameConfig<Model::Enum::Value>,
		NameConfig<Model::Class>,
		NameConfig<Model::Class::Constant>,
		NameConfig<Model::Class::Event>,
		NameConfig<Model::Class::Operation>
	> NameConfigs;

	struct PrimitiveConfigBase
	{
		std::string native;

		PrimitiveConfigBase(std::string native)
			: native(native)
		{ }

		PrimitiveConfigBase(const PrimitiveConfigBase& other)
			: native(other.native)
		{ }

		PrimitiveConfigBase(PrimitiveConfigBase&& other)
			: native(std::move(other.native))
		{ }
	};

	template<Model::Primitive::Underlying U>
	struct PrimitiveConfig : public PrimitiveConfigBase
	{
		using PrimitiveConfigBase::PrimitiveConfigBase;
	};

	typedef std::tuple<
		PrimitiveConfig<Model::Primitive::Underlying::BYTE>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT16>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT32>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT64>,
		PrimitiveConfig<Model::Primitive::Underlying::BOOLEAN>,
		PrimitiveConfig<Model::Primitive::Underlying::TIMESTAMP>,
		PrimitiveConfig<Model::Primitive::Underlying::STRING>,
		PrimitiveConfig<Model::Primitive::Underlying::UUID>,
		PrimitiveConfig<Model::Primitive::Underlying::BUFFER>,
		PrimitiveConfig<Model::Primitive::Underlying::CONST_BUFFER>,
		PrimitiveConfig<Model::Primitive::Underlying::VECTOR>,
		PrimitiveConfig<Model::Primitive::Underlying::LIST>,
		PrimitiveConfig<Model::Primitive::Underlying::SET>,
		PrimitiveConfig<Model::Primitive::Underlying::MAP>
	> PrimitiveConfigs;

	std::string namespaceDelimiter;
	std::string indentData;
	std::uint8_t documentationWrapping;
	NameConfigs nameConfigs;
	PrimitiveConfigs primitiveConfigs;

	FormatterConfig(std::string namespaceDelimiter, std::string indentData, std::uint8_t documentationWrapping, NameConfigs nameConfigs, PrimitiveConfigs primitiveConfigs)
		: namespaceDelimiter(namespaceDelimiter)
		, indentData(indentData)
		, documentationWrapping(documentationWrapping)
		, nameConfigs(nameConfigs)
		, primitiveConfigs(primitiveConfigs)
	{ }

	FormatterConfig(const FormatterConfig& other)
		: namespaceDelimiter(other.namespaceDelimiter)
		, indentData(other.indentData)
		, documentationWrapping(other.documentationWrapping)
		, nameConfigs(other.nameConfigs)
		, primitiveConfigs(other.primitiveConfigs)
	{ }

	FormatterConfig(FormatterConfig&& other)
		: namespaceDelimiter(std::move(other.namespaceDelimiter))
		, indentData(std::move(other.indentData))
		, documentationWrapping(std::move(other.documentationWrapping))
		, nameConfigs(std::move(other.nameConfigs))
		, primitiveConfigs(std::move(other.primitiveConfigs))
	{ }

	template<typename T>
	NameConfig<T> nameConfig() const
	{
		return TupleHelper::get<NameConfig<T>>(nameConfigs);
	}

	template<Model::Primitive::Underlying U>
	PrimitiveConfig<U> primitiveConfig() const
	{
		return TupleHelper::get<PrimitiveConfig<U>>(primitiveConfigs);
	}
};

} } // namespace: Everbase::InterfaceCompiler