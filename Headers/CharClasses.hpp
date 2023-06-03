/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

/************************************************************************/

namespace HTMLParser
{
    namespace CharClasses
    {
        bool isWhitespace(char32_t);
        bool isTagname(char32_t);
    }
}

/************************************************************************/

bool HTMLParser::CharClasses::isWhitespace(char32_t c)
{
    return c==0x0020 || c==0x0009 || c==0x000a || c==0x000c || c==0x000d;
}

/************************************************************************/

bool HTMLParser::CharClasses::isTagname(char32_t c)
{
    return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}
