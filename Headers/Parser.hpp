/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Buffer.hpp"
#include "./Doctype.hpp"

/************************************************************************/

namespace HTMLParser
{
    class Parser
    {
    private:
        Buffer buffer;

    public:
        Parser(std::string_view);
        ~Parser() =default;

    public:
        void parse();
    };
}

/************************************************************************/

inline HTMLParser::Parser::Parser(std::string_view data)
    : buffer(data)
{
    Tokens::skipCommentsAndSpace(buffer);
    skipDoctype(buffer);
    Tokens::skipCommentsAndSpace(buffer);
}
