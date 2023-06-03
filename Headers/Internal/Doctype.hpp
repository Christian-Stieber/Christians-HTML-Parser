/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * Note: I'm only checking for <!doctype html ...>
 */

inline void HTMLParser::Parser::skipDoctype()
{
    needs(skipString("<!doctype") &&
          skipWhitespace() &&
          skipString("html"));
    auto c=buffer.getChar();
    needs(c=='>' || isWhitespace(c));
    while (c!='>')
    {
        c=buffer.getChar();
    }
}
