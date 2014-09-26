#ifndef BASICINDENT_HPP
#define BASICINDENT_HPP

#include "formatter/stream/basicFormatting.hpp"

namespace Api { namespace Formatter { namespace Stream {

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_indent : public basic_formatting<CharT, Traits>
{
public:
    basic_indent( std::basic_ostream<CharT, Traits>& stream, std::basic_string<CharT, Traits> data = "    " )
        : basic_formatting<CharT, Traits>( stream )
        , _bol( true )
        , _data( data )
    {
    }

    virtual ~basic_indent()
    {
    }

protected:
    using typename basic_formatting<CharT, Traits>::int_type;

    virtual int_type overflow( int_type ch ) override
    {
        if ( this->enabled() && _bol )
        {
            (*this).dest().sputn( _data.data(), _data.size() );
        }
        _bol = ch == '\n';
        return (*this).dest().sputc( ch );
    }

    virtual int depth() override
    {
        return this->basic_formatting<CharT, Traits>::depth() + _data.length();
    }

private:
    std::basic_string<CharT, Traits> _data;
    bool _bol;
};

using indent = basic_indent<char>;
using windent = basic_indent<wchar_t>;

} } } // namespace Api::Formatter::Stream

#endif // BASICINDENT_HPP
