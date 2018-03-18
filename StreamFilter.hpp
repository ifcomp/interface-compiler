#pragma once

#include <streambuf>
#include <ostream>
#include <string>
#include <list>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace StreamFilter {

/**
 * @brief Base class
 */

template<class CharT, class Traits = std::char_traits<CharT>>
class BasicFilterBuffer : public std::basic_streambuf<CharT, Traits>
{
public:
    BasicFilterBuffer( std::basic_streambuf<CharT, Traits>& dest )
        : _dest( dest )
        , _enabled( true )
    {
    }

    virtual ~BasicFilterBuffer()
    {
    }

protected:
    using Base = std::basic_streambuf<CharT, Traits>;
    using typename Base::int_type;
    using typename Base::char_type;

    virtual int_type overflow( int_type ch ) override final
    {
        if(_enabled)
        {
            return output(_dest, ch);
        }
        else
        {
            return _dest.sputc( ch );
        }

        return ch;
    }

protected:
    virtual int_type output( std::basic_streambuf<CharT, Traits>& dest, char_type ch )
    {
        return dest.sputc( ch );
    }

    virtual int depth()
    {
        if( auto d = dynamic_cast<BasicFilterBuffer<CharT, Traits>*>(&_dest) )
        {
            return d->depth();
        }

        return 0;
    }

protected:
    void enable(bool status)
    {
        _enabled = status;

        if( auto d = dynamic_cast<BasicFilterBuffer<CharT, Traits>*>(&_dest) )
        {
            d->enable(status);
        }
    }

private:
    std::basic_streambuf<CharT, Traits>& _dest;
    bool _enabled;
};


template<class CharT, class Traits = std::char_traits<CharT>>
class BasicFilterStream : public std::basic_ostream<CharT, Traits>
{
private:
    using FilterBuffer = BasicFilterBuffer<CharT, Traits>;
    using FilterBufferRef = std::unique_ptr<FilterBuffer>;
    std::list<FilterBufferRef> _buffers;

public:
    BasicFilterStream(std::basic_ostream<CharT, Traits>& dest)
        : std::basic_ostream<CharT, Traits>(new FilterBuffer(*(dest.rdbuf())))
    {
        // add non-filtering buffer
        _buffers.push_back(FilterBufferRef(static_cast<FilterBuffer*>(this->rdbuf())));
        this->rdbuf(_buffers.back().get());
    }

    virtual ~BasicFilterStream()
    {
        // flush and clean up in the right order
        this->flush();
        this->rdbuf(nullptr);

        while(_buffers.size() > 0)
        {
            _buffers.pop_back();
        }
    }

    template<template<typename, typename> class BufferT, typename... ArgsT>
    BasicFilterStream<CharT, Traits>& push(ArgsT&&... args)
    {
        _buffers.push_back(FilterBufferRef(new BufferT<CharT, Traits>(*(_buffers.back().get()), std::forward<ArgsT>(args)...)));
        this->rdbuf(_buffers.back().get());
        return *this;
    }

    BasicFilterStream<CharT, Traits>& pop()
    {
        if(_buffers.size() > 0)
        {
            _buffers.pop_back();
            this->rdbuf(_buffers.back().get());
        }
        else
        {
            throw std::range_error("no filters on stack");
        }

        return *this;
    }
};

using filter = BasicFilterStream < char > ;
using wfilter = BasicFilterStream < wchar_t > ;



/**
 * @brief Indent
 */

template<class CharT, class Traits = std::char_traits<CharT>, std::size_t COUNT = 4, CharT WHITESPACE = ' '>
class BasicIndentBuffer : public BasicFilterBuffer<CharT, Traits>
{
public:
    BasicIndentBuffer( std::basic_streambuf<CharT, Traits>& dest, std::basic_string<CharT, Traits> prefix, std::size_t count, CharT whitespace = WHITESPACE, bool bol = false )
        : BasicFilterBuffer<CharT, Traits>( dest )
        , _prefix( prefix )
        , _data( std::basic_string<CharT, Traits>( count, whitespace ) )
        , _bol( bol )
    {
    }

    BasicIndentBuffer( std::basic_streambuf<CharT, Traits>& dest, std::basic_string<CharT, Traits> data )
        : BasicFilterBuffer<CharT, Traits>( dest )
        , _data( data )
        , _bol( true )
    {
    }

    BasicIndentBuffer( std::basic_streambuf<CharT, Traits>& dest, std::size_t count = COUNT, CharT whitespace = WHITESPACE )
        : BasicIndentBuffer( dest, std::basic_string<CharT, Traits>( count, whitespace ) )
    {
    }

    BasicIndentBuffer( std::basic_streambuf<CharT, Traits>& dest, const char* data )
        : BasicIndentBuffer( dest, std::string( data ) )
    {
    }

    virtual ~BasicIndentBuffer()
    {
    }

protected:
    using Base = BasicFilterBuffer<CharT, Traits>;
    using typename Base::int_type;
    using typename Base::char_type;

    virtual int_type output( std::basic_streambuf<CharT, Traits>& dest, char_type ch ) override
    {
        if ( _bol )
        {
            dest.sputn( _prefix.data(), _prefix.size() );
            dest.sputn( _data.data(), _data.size() );
        }
        _bol = ch == '\n';
        return dest.sputc( ch );
    }

    virtual int depth() override
    {
        return this->BasicFilterBuffer<CharT, Traits>::depth() + _prefix.length() + _data.length();
    }

private:
    std::basic_string<CharT, Traits> _prefix;
    std::basic_string<CharT, Traits> _data;
    bool _bol;
};

template<class CharT, class Traits = std::char_traits<CharT>>
using indent = BasicIndentBuffer<CharT, Traits>;


/**
 * @brief Wrap
 */

template<class CharT, class Traits = std::char_traits<CharT>>
class BasicWrapBuffer : public BasicFilterBuffer<CharT, Traits>
{
public:
    BasicWrapBuffer( std::basic_streambuf<CharT, Traits>& dest, int length = 75 )
        : BasicFilterBuffer<CharT, Traits>( dest )
        , _length( length )
        , _current( this->depth() )
    {
    }

    virtual ~BasicWrapBuffer()
    {
    }

protected:
    using Base = BasicFilterBuffer<CharT, Traits>;
    using typename Base::int_type;
    using typename Base::char_type;

    virtual int_type output( std::basic_streambuf<CharT, Traits>& dest, char_type ch ) override
    {
        if (_current >= _length && ch == ' ')
        {
            // line is longer than allowed --> search backwards for last delimiter
            size_t pos = _data.find_last_of(' ');

            if (pos == std::string::npos)
            {
                // no whitespace found --> output whole line
                dest.sputn( _data.data(), _data.size() );
                _data.clear();
            }
            else
            {
                // output until pos and put rest into _data
                dest.sputn( _data.data(), pos );
                _data = _data.substr(pos + 1);
                _data.push_back(ch);
            }

            dest.sputc('\n');
            _current = this->depth() + _data.size();
        }
        else
        {
            if ( ch == '\n' )
            {
                _data.push_back(ch);
                dest.sputn( _data.data(), _data.size() );
                _data.clear();
                _current = this->depth();
            }
            else
            {
                _data.push_back(ch);
                _current += 1;
            }
        }

        return 0;
    }

private:
    std::basic_string<CharT, Traits> _data;
    int _length;
    int _current;
};


template<class CharT, class Traits = std::char_traits<CharT>>
using wrap = BasicWrapBuffer < CharT, Traits > ;


/**
 * @brief Reset
 */

template<class CharT, class Traits = std::char_traits<CharT>>
class BasicResetBuffer : public BasicFilterBuffer<CharT, Traits>
{
public:
    BasicResetBuffer( std::basic_streambuf<CharT, Traits>& dest )
        : BasicFilterBuffer<CharT, Traits>( dest )
    {
        this->enable(false);
    }

    virtual ~BasicResetBuffer()
    {
        this->enable(true);
    }
};

template<class CharT, class Traits = std::char_traits<CharT>>
using reset = BasicResetBuffer < CharT, Traits > ;


/**
 * @brief Counter
 */

template<class CharT, class Traits = std::char_traits<CharT>>
class BasicCounterBuffer : public BasicFilterBuffer<CharT, Traits>
{
public:
    BasicCounterBuffer( std::basic_streambuf<CharT, Traits>& dest, std::size_t& count )
        : BasicFilterBuffer<CharT, Traits>( dest )
        , _count( count )
    {
    }

    virtual ~BasicCounterBuffer()
    {
    }

protected:
    using Base = BasicFilterBuffer<CharT, Traits>;
    using typename Base::int_type;
    using typename Base::char_type;

    virtual int_type output( std::basic_streambuf<CharT, Traits>& dest, char_type ch ) override
    {
        _count += 1;
        return dest.sputc( ch );
    }

private:
    std::size_t& _count;
};

template<class CharT, class Traits = std::char_traits<CharT>>
using counter = BasicCounterBuffer < CharT, Traits > ;

} } } // namespace: Everbase::InterfaceCompiler::StreamFilter
