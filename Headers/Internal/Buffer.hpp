/*
 * Copyright (C) 2023- Christian Stieber
 */

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
    : buffer(UTF8Codecvt::convert(data))
{
}

/************************************************************************/

inline char32_t HTMLParser::Buffer::getChar()
{
    if (index<buffer.size())
    {
        auto c=buffer[index++];
        std::cout << (char)c << std::flush;
        return c;
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
 * Saves the buffer position. Calls callback(). If callback
 * returns true, does nothing. If callback returns false, restores
 * position. Returns the callback result.
 */

template <typename CALLBACK> bool HTMLParser::Buffer::savePosition(CALLBACK callback)
{
    const auto saved=index;
    auto result=callback();
    if (!result)
    {
        index=saved;
    }
    return result;
}
