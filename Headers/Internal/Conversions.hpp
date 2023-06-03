/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * For now(?), this only works with A-Z :-)
 */

inline char32_t HTMLParser::Parser::toLower(char32_t c)
{
    return (c>='A' && c<='Z') ? c|0x20 : c;
}
