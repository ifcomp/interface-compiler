#ifndef BASICFORMATTING_HPP
#define BASICFORMATTING_HPP

#include <streambuf>
#include <ostream>
#include <string>
#include <typeinfo>

namespace Api { namespace Formatter { namespace Stream {

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_formatting : public std::basic_streambuf<CharT, Traits>
{
public:
    basic_formatting( std::basic_ostream<CharT, Traits>& stream )
        : _stream( stream )
        , _dest( stream.rdbuf() )
        , _enabled( true )
    {
        _stream.rdbuf( this );
    }

    virtual ~basic_formatting()
    {
        _stream.rdbuf( _dest );
    }

protected:
    using typename std::basic_streambuf<CharT, Traits>::int_type;

    virtual int_type overflow( int_type ch ) override
    {
        return (*this).dest().sputc( ch );
    }

    template<class FormattingT>
    void enable(bool status)
    {
        if(typeid(*this) == typeid(FormattingT))
        {
            _enabled = status;
        }

        if( auto d = dynamic_cast<basic_formatting<CharT, Traits>*>(_dest) )
        {
            d->template enable<FormattingT>(status);
        }
    }

    virtual int depth()
    {
        if( auto d = dynamic_cast<basic_formatting<CharT, Traits>*>(_dest) )
        {
            return d->depth();
        }

        return 0;
    }

protected:
    std::basic_streambuf<CharT, Traits>& dest()
    {
        return *_dest;
    }

    bool enabled()
    {
        return _enabled;
    }

private:
    std::basic_ostream<CharT, Traits>& _stream;
    std::basic_streambuf<CharT, Traits>* _dest;
    bool _enabled;
};

} } } // namespace Api::Formatter::Stream

#endif // BASICFORMATTING_HPP
