/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * https://www.w3.org/TR/2012/WD-html-markup-20120329/Overview.html
 */

/************************************************************************/

#pragma once

#if __cplusplus < 202002L
#error This was only tested with C++20
#else

#include <unordered_map>
#include <memory>
#include <cassert>
#include <string>

#include "./Exceptions.hpp"
#include "./Encoding.hpp"
#include "./Buffer.hpp"
#include "./Tree.hpp"

/************************************************************************/

namespace HTMLParser
{
    class Parser
    {
    private:
        Buffer buffer;

    private:
        Tree::Document document;

    public:
        Parser(std::string_view);
        ~Parser() =default;

    public:
        void parse();

    private:
        /* CharClasses.hpp */
        static bool isWhitespace(char32_t);
        static bool isTagname(char32_t);
        static bool isAttributeName(char32_t);
        static bool isUnquotedAttributeValue(char32_t);
        static bool isDoubleQuotedAttributeValue(char32_t);
        static bool isSingleQuotedAttributeValue(char32_t);
        static bool isCharacterReferenceName(char32_t);

    private:
        /* Conversions.hpp */
        static char32_t toLower(char32_t);

    private:
        /* Tokens.hpp */
        bool skipWhitespace();
        bool skipComment();
        bool skipCommentsAndSpace();
        bool skipString(const char*);
        std::string getSomeString(bool(*)(char32_t), bool);
        std::string getTagname();
        std::string getAttributeName();

    private:
        /* Doctype.hpp */
        void skipDoctype();

    private:
        /* CharacterReference.hpp */
        static uint32_t namedReference(std::string_view);	/* internal */
        char32_t getCharacterReference();

    private:
        /* CharacterData.hpp */
        std::string getNormalCharacterData();
        std::string getSpecialCharacterData(bool, const char*);

    private:
        /* Attributes.hpp */
        std::pair<std::string, std::string> getAttribute();

    private:
        /* Element.hpp */
        std::unique_ptr<HTMLParser::Tree::Text> getNormalElementText();		/* internal */
        void getSpecialElementText(Tree::Element&, bool);	/* internal */
        static bool isVoidElement(std::string_view);	/* internal */
        void elementContent(Tree::Element&);			/* internal */
        void endTag(Tree::Element&);					/* internal */
        std::unique_ptr<Tree::Element> openElement();	/* internal */
        std::unique_ptr<Tree::Element> startTag();		/* internal */
        std::unique_ptr<Tree::Element> getElement();

    private:
        /* Text.hpp */
        std::unique_ptr<Tree::Text> getText();
    };
}

/************************************************************************/

#include "./CharClasses.hpp"
#include "./Conversions.hpp"
#include "./Tokens.hpp"
#include "./CharacterReference.hpp"
#include "./CharacterData.hpp"

#include "./Doctype.hpp"
#include "./Attribute.hpp"
#include "./Element.hpp"

/************************************************************************/

inline HTMLParser::Parser::Parser(std::string_view data)
    : buffer(data)
{
    skipCommentsAndSpace();
    skipDoctype();
    skipCommentsAndSpace();
    getElement();
}

/************************************************************************/

#endif /* __cplusplus */
