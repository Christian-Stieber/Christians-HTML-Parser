/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

bool HTMLParser::Parser::isWhitespace(char32_t c)
{
    return c==0x0020 || c==0x0009 || c==0x000a || c==0x000c || c==0x000d;
}

/************************************************************************/

bool HTMLParser::Parser::isTagname(char32_t c)
{
    return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}
