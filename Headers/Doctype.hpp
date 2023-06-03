/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Tokens.hpp"

/************************************************************************/

namespace HTMLParser
{
    void skipDoctype(Buffer&);
}

/************************************************************************/
/*
 * Note: I'm only checking for <!doctype html ...>
 */

inline void HTMLParser::skipDoctype(Buffer& buffer)
{
    needs(Tokens::skipString(buffer, "<!doctype") &&
          Tokens::skipWhitespace(buffer) &&
          Tokens::skipString(buffer, "html"));
    auto c=buffer.getChar();
    needs(c=='>' || CharClasses::isWhitespace(c));
    while (c!='>')
    {
        c=buffer.getChar();
    }
    buffer.ungetChar();
}
