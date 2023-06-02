#pragma once

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
    public:
        class EOFException { };
        class ConversionException { };

    private:
        std::u32string buffer;
        size_t index=0;

    public:
        Buffer(std::string_view);
        ~Buffer() =default;

    public:
        char32_t getChar();
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
/*
 * Returns a single character.
 *
 * Throws EOFException.
 */

inline char32_t HTMLParser::Buffer::getChar()
{
    if (index<buffer.size())
    {
        return buffer[index++];
    }
    throw EOFException();
}
