/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Exceptions.hpp"
#include "./Buffer.hpp"

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

    private:
        /* CharClasses.hpp */
        static bool isWhitespace(char32_t);
        static bool isTagname(char32_t);

    private:
        /* Conversions.hpp */
        static char32_t toLower(char32_t);

    private:
        /* Tokens.hpp */
        bool skipWhitespace();
        bool skipComment();
        bool skipCommentsAndSpace();
        bool skipString(const char*);
        std::string getTagname();

    private:
        /* Doctype.hpp */
        void skipDoctype();
    };
}

/************************************************************************/

#include "./CharClasses.hpp"
#include "./Conversions.hpp"
#include "./Tokens.hpp"

#include "./Doctype.hpp"

/************************************************************************/

inline HTMLParser::Parser::Parser(std::string_view data)
    : buffer(data)
{
    skipCommentsAndSpace();
    skipDoctype();
    skipCommentsAndSpace();
}
