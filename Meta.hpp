#pragma once

#include <cstddef>

namespace Everbase { namespace InterfaceCompiler { namespace Meta {

template< std::size_t ... I>
struct index_sequence
{
};

template< std::size_t s, typename I1, typename I2>
struct concate;

template< std::size_t s, std::size_t ...I, std::size_t ...J>
struct concate<s, index_sequence<I...>, index_sequence<J...> >
{
    typedef index_sequence<I... ,( J + s)... > type;
};


template< std::size_t N>
struct make_index_sequence
{
    typedef typename concate< N/2,
       typename make_index_sequence< N/2>::type,
       typename make_index_sequence< N - N/2>::type
    >::type type;
};

template<>struct make_index_sequence<0>
{
    typedef index_sequence<> type;
};

template<> struct make_index_sequence<1>
{
    typedef index_sequence<0> type;
};

template< typename ...T>
using index_sequence_for = typename make_index_sequence< sizeof...(T) > ::type;

} } } // namespace: Everbase::InterfaceCompiler::Meta
