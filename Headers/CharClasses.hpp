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

/************************************************************************/
/*
 * Note: apparently, attribute names can be pretty much anything,
 * but I'm not sure whether I want to believe that.
 */

bool HTMLParser::Parser::isAttributeName(char32_t c)
{
    if (isWhitespace(c)) return false;
    if ((c>=0x00 && c<=0x1f) || (c>=0x80 && c<=0x9f)) return false;
    if (c=='"' || c=='\'' || c=='>' || c=='/' || c=='=') return false;
    return true;
}

/************************************************************************/

bool HTMLParser::Parser::isUnquotedAttributeValue(char32_t c)
{
    if (isWhitespace(c)) return false;
    if (c=='"' || c=='\'' || c=='=' || c=='>' || c=='<' || c=='`') return false;
    return true;
}

/************************************************************************/

bool HTMLParser::Parser::isDoubleQuotedAttributeValue(char32_t c)
{
    return c!='"';
}

/************************************************************************/

bool HTMLParser::Parser::isSingleQuotedAttributeValue(char32_t c)
{
    return c!='\'';
}
