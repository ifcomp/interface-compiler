#pragma once

#include "Meta.hpp"

#include <cstddef>
#include <tuple>
#include <ostream>

namespace Everbase { namespace InterfaceCompiler {

class Formatter;

template<typename... ArgsT>
struct FormatToken
{
	const Formatter* formater;
	void(Formatter::*function)(std::ostream&, ArgsT...) const;
	std::tuple<ArgsT...> parameters;

	void call(std::ostream& stream);

private:
	template<std::size_t... I>
	void call(std::ostream& stream, Meta::index_sequence<I...>);
};

template<typename... ArgsT>
void FormatToken<ArgsT...>::call(std::ostream& stream)
{
	call(stream, Meta::index_sequence_for<ArgsT...>{});
}

template<typename... ArgsT>
template<std::size_t... I>
void FormatToken<ArgsT...>::call(std::ostream& stream, Meta::index_sequence<I...>)
{
	(formater->*function)(stream, std::get<I>(parameters)...);
}

template<typename... ArgsT>
std::ostream& operator<<(std::ostream& stream, FormatToken<ArgsT...> token)
{
	token.call(stream);
	return stream;
}

} } // namespace: Everbase::InterfaceCompiler
