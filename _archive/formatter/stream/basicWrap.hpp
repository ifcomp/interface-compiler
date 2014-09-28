#ifndef BASICWRAP_HPP
#define BASICWRAP_HPP

#include "formatter/stream/basicFormatting.hpp"

namespace Api { namespace Formatter { namespace Stream {

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_wrap : public basic_formatting<CharT, Traits>
{
public:
    basic_wrap( std::basic_ostream<CharT, Traits>& stream, int length = 75 )
        : basic_formatting<CharT, Traits>( stream )
        , _length( length )
        , _current( this->depth() )
    {
    }

    virtual ~basic_wrap()
    {
    }

protected:
    using typename basic_formatting<CharT, Traits>::int_type;

    virtual int_type overflow( int_type ch ) override
    {
        if( ch != '\n' && _current >= _length)
        {
            (*this).dest().sputc( '\n' );
            _current = this->depth();
        }

        if( ch == '\n' )
        {
            _current = this->depth();
        }

        _current += 1;

        return (*this).dest().sputc( ch );
    }

private:
    int _length;
    int _current;
};

using wrap = basic_wrap<char>;
using wwrap = basic_wrap<wchar_t>;

} } } // namespace Api::Formatter::Stream

#endif // BASICWRAP_HPP
