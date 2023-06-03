/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

inline bool HTMLParser::Parser::skipWhitespace()
{
    bool skipped=false;
    while (isWhitespace(buffer.getChar()))
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

inline bool HTMLParser::Parser::skipString(const char* string)
{
    return buffer.savePosition([this, string]() mutable {
        while (*string!='\0')
        {
            if (toLower(buffer.getChar())!=*string)
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

inline std::string HTMLParser::Parser::getTagname()
{
    std::string name;
    char32_t c;
    while (isTagname(c=buffer.getChar()))
    {
        name.push_back(toLower(c));
    }
    buffer.ungetChar();
    return name;
}

/************************************************************************/
/*
 * Returns true if we've skipped a comment
 */

inline bool HTMLParser::Parser::skipComment()
{
    if (!buffer.savePosition([this](){ return skipString("<!--"); }))
    {
        return false;
    }
    while (!skipString("-->"))
    {
        buffer.getChar();
    }
    return true;
}

/************************************************************************/

inline bool HTMLParser::Parser::skipCommentsAndSpace()
{
    bool skipped=false;
    while (skipWhitespace() || skipComment())
    {
        skipped=true;
    }
    return skipped;
}
