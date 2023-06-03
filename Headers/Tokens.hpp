/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Buffer.hpp"
#include "./CharClasses.hpp"
#include "./Conversions.hpp"

/************************************************************************/

namespace HTMLParser
{
    namespace Tokens
    {
        bool skipWhitespace(Buffer&);
        bool skipComment(Buffer&);
        bool skipCommentsAndSpace(Buffer&);
        bool skipString(Buffer&, const char*);

        std::string getTagname(Buffer&);
    }
}

/************************************************************************/

inline bool HTMLParser::Tokens::skipWhitespace(HTMLParser::Buffer& buffer)
{
    bool skipped=false;
    while (CharClasses::isWhitespace(buffer.getChar()))
    {
        skipped=true;
    }
    buffer.ungetChar();
    return skipped;
}

/************************************************************************/
/*
 * Note: turns characters to lowercase before matching
 */

inline bool HTMLParser::Tokens::skipString(Buffer& buffer, const char* string)
{
    return buffer.savePosition([string](Buffer& buffer) mutable {
        while (*string!='\0')
        {
            if (Conversions::toLower(buffer.getChar())!=*string)
            {
                return false;
            }
            string++;
        };
        return true;
    });
}

/************************************************************************/
/*
 * Note: returns lowercase names, or empty if no tagname
 */

inline std::string HTMLParser::Tokens::getTagname(Buffer& buffer)
{
    std::string name;
    char32_t c;
    while (CharClasses::isTagname(c=buffer.getChar()))
    {
        name.push_back(Conversions::toLower(c));
    }
    buffer.ungetChar();
    return name;
}

/************************************************************************/
/*
 * Returns true if we've skipped a comment
 */

inline bool HTMLParser::Tokens::skipComment(Buffer& buffer)
{
    if (!buffer.savePosition([](Buffer& buffer){ return skipString(buffer,"<!--"); }))
    {
        return false;
    }
    while (!skipString(buffer,"-->"))
    {
        buffer.getChar();
    }
    return true;
}

/************************************************************************/

inline bool HTMLParser::Tokens::skipCommentsAndSpace(Buffer& buffer)
{
    bool skipped=false;
    while (skipWhitespace(buffer) || skipComment(buffer))
    {
        skipped=true;
    }
    return skipped;
}
