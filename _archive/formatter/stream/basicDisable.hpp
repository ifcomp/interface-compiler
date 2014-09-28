#ifndef BASICDISABLE_HPP
#define BASICDISABLE_HPP

#include "formatter/stream/basicFormatting.hpp"

namespace Api { namespace Formatter { namespace Stream {

template<template<class CharT, class Traits> class FormattingT, class CharT, class Traits = std::char_traits<CharT>>
class basic_disable : public basic_formatting<CharT, Traits>
{
public:
    basic_disable( std::basic_ostream<CharT, Traits>& stream )
        : basic_formatting<CharT, Traits>( stream )
    {
        this->template enable<FormattingT<CharT, Traits>>(false);
    }

    virtual ~basic_disable()
    {
        this->template enable<FormattingT<CharT, Traits>>(true);
    }
};

using noindent = basic_disable<basic_indent, char>;
using wnoindent = basic_disable<basic_indent, wchar_t>;

using nowrap = basic_disable<basic_wrap, char>;
using wnowrap = basic_disable<basic_wrap, wchar_t>;

} } } // namespace Api::Formatter::Stream

#endif // BASICDISABLE_HPP
