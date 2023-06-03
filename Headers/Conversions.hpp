/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

/************************************************************************/

namespace HTMLParser
{
    namespace Conversions
    {
        char32_t toLower(char32_t);
    }
}

/************************************************************************/
/*
 * For now(?), this only works with A-Z :-)
 */

inline char32_t HTMLParser::Conversions::toLower(char32_t c)
{
    return (c>='A' && c<='Z') ? c|0x20 : c;
}
