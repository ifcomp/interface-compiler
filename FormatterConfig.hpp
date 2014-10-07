#pragma once

#include "Model/Model.hpp"

#include <boost/tuple/tuple.hpp>

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
	T& get(boost::tuple<Types...> tuple)
	{
	    return boost::get<get_internal<0,T,Types...>::type::index>(tuple);
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

	typedef boost::tuple<
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

	typedef boost::tuple<
		PrimitiveConfig<Model::Primitive::Underlying::BYTE>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT16>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT32>,
		PrimitiveConfig<Model::Primitive::Underlying::UINT64>,
		PrimitiveConfig<Model::Primitive::Underlying::BOOLEAN>,
		PrimitiveConfig<Model::Primitive::Underlying::TIMESTAMP>,
		PrimitiveConfig<Model::Primitive::Underlying::STRING>,
		PrimitiveConfig<Model::Primitive::Underlying::UUID>
	> PrimitiveConfigs1;

//	typedef boost::tuple<
//		PrimitiveConfig<Model::Primitive::Underlying::BUFFER>,
//		PrimitiveConfig<Model::Primitive::Underlying::CONST_BUFFER>
//	> PrimitiveConfigs2;

	typedef boost::tuple<
		PrimitiveConfig<Model::Primitive::Underlying::VECTOR>,
		PrimitiveConfig<Model::Primitive::Underlying::LIST>,
		PrimitiveConfig<Model::Primitive::Underlying::SET>,
		PrimitiveConfig<Model::Primitive::Underlying::MAP>
	> PrimitiveConfigs3;

	typedef std::vector<std::string> StripVerbs;

	std::string namespaceDelimiter;
	std::string indentData;
	std::uint8_t documentationWrapping;
	NameConfigs nameConfigs;
	PrimitiveConfigs1 primitiveConfigs1;
//	PrimitiveConfigs2 primitiveConfigs2;
	PrimitiveConfigs3 primitiveConfigs3;
	StripVerbs stripVerbs;

	FormatterConfig(std::string namespaceDelimiter, std::string indentData, std::uint8_t documentationWrapping, NameConfigs nameConfigs, PrimitiveConfigs1 primitiveConfigs1, /*PrimitiveConfigs2 primitiveConfigs2,*/ PrimitiveConfigs3 primitiveConfigs3, StripVerbs stripVerbs)
		: namespaceDelimiter(namespaceDelimiter)
		, indentData(indentData)
		, documentationWrapping(documentationWrapping)
		, nameConfigs(nameConfigs)
		, primitiveConfigs1(primitiveConfigs1)
//		, primitiveConfigs2(primitiveConfigs2)
		, primitiveConfigs3(primitiveConfigs3)
		, stripVerbs(stripVerbs)
	{ }

	FormatterConfig(const FormatterConfig& other)
		: namespaceDelimiter(other.namespaceDelimiter)
		, indentData(other.indentData)
		, documentationWrapping(other.documentationWrapping)
		, nameConfigs(other.nameConfigs)
		, primitiveConfigs1(other.primitiveConfigs1)
//		, primitiveConfigs2(other.primitiveConfigs2)
		, primitiveConfigs3(other.primitiveConfigs3)
		, stripVerbs(other.stripVerbs)
	{ }

	FormatterConfig(FormatterConfig&& other)
		: namespaceDelimiter(std::move(other.namespaceDelimiter))
		, indentData(std::move(other.indentData))
		, documentationWrapping(std::move(other.documentationWrapping))
		, nameConfigs(std::move(other.nameConfigs))
		, primitiveConfigs1(std::move(other.primitiveConfigs1))
//		, primitiveConfigs2(std::move(other.primitiveConfigs2))
		, primitiveConfigs3(std::move(other.primitiveConfigs3))
		, stripVerbs(std::move(other.stripVerbs))
	{ }

	template<typename T>
	NameConfig<T> nameConfig() const
	{
		return TupleHelper::get<NameConfig<T>>(nameConfigs);
	}

	template<Model::Primitive::Underlying U>
	PrimitiveConfig<U> primitiveConfig1() const
	{
		return TupleHelper::get<PrimitiveConfig<U>>(primitiveConfigs1);
	}

//	template<Model::Primitive::Underlying U>
//	PrimitiveConfig<U> primitiveConfig2() const
//	{
//		return TupleHelper::get<PrimitiveConfig<U>>(primitiveConfigs2);
//	}

	template<Model::Primitive::Underlying U>
	PrimitiveConfig<U> primitiveConfig3() const
	{
		return TupleHelper::get<PrimitiveConfig<U>>(primitiveConfigs3);
	}
};

} } // namespace: Everbase::InterfaceCompiler
