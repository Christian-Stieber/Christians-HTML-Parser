/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Exceptions.hpp"

#include <string>
#include <locale>
#include <cassert>
#include <cstring>

/************************************************************************/
/*
 * This supplies the character data for our parser.
 *
 * In theory, this is where the encoding would be handled as well.
 * In practice, I've hardcoded UTF-8 for now.
 */

namespace HTMLParser
{
    class Buffer
    {
    private:
        std::u32string buffer;
        size_t index=0;

    public:
        Buffer(std::string_view);
        ~Buffer() =default;

    public:
        char32_t getChar();
        void ungetChar();

    public:
        template <typename CALLBACK> bool savePosition(CALLBACK);
    };
}

/************************************************************************/
/*
 * Assumes the data is UTF-8, and converts it to UCS-4.
 *
 * Throws ConversionException.
*/

inline HTMLParser::Buffer::Buffer(std::string_view data)
{
    buffer.resize(data.size());

    const char8_t* from=static_cast<const char8_t*>(static_cast<const void*>(data.data()));
    const char8_t* from_end=static_cast<const char8_t*>(static_cast<const void*>(data.data()+data.size()));
    const char8_t* from_next;
    char32_t* to_next;

    class Codecvt : public std::codecvt<char32_t, char8_t, std::mbstate_t>
    {
    public:
        Codecvt(): codecvt(1) { }
        virtual ~Codecvt() =default;
    };

    Codecvt codecvt;
    Codecvt::state_type state;
    memset(&state, 0, sizeof(state));
    auto result=codecvt.in(state, from, from_end, from_next, buffer.data(), buffer.data()+buffer.size(), to_next);

    if (result==std::codecvt_base::result::ok &&
        from_next==from_end &&
        mbsinit(&state))
    {
        buffer.resize(to_next-buffer.data());
    }
    else
    {
        assert(result!=std::codecvt_base::result::noconv);
        throw ConversionException();
    }
}

/************************************************************************/

inline char32_t HTMLParser::Buffer::getChar()
{
    if (index<buffer.size())
    {
        return buffer[index++];
    }
    throw EOFException();
}

/************************************************************************/

inline void HTMLParser::Buffer::ungetChar()
{
    assert(index>0);
    index--;
}

/************************************************************************/
/*
 * Saves the buffer position. Calls callback(Buffer&). If callback
 * returns true, does nothing. If callback returns false, restores
 * position. Returns the callback result.
 */

template <typename CALLBACK> bool HTMLParser::Buffer::savePosition(CALLBACK callback)
{
    const auto saved=index;
    auto result=callback(*this);
    if (!result)
    {
        index=saved;
    }
    return result;
}
