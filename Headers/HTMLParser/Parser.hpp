/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * https://www.w3.org/TR/2012/WD-html-markup-20120329/Overview.html
 *
 * Note: it seems I wasn't very good at reading it properly, as I kept
 * making adjustments to deal with actual websites...
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
#include <utility>

#include "./Tree.hpp"

#include "./Internal/Exceptions.hpp"
#include "./Internal/Encoding.hpp"
#include "./Internal/Buffer.hpp"

/************************************************************************/
/*
 * Create a Parser object with the UTF-8 data.
 *
 * If successful (i.e. doesn't throw), get the document member.
 */

namespace HTMLParser
{
    class Parser
    {
    private:
        Buffer buffer;

    public:
        Parser(std::string_view);
        virtual ~Parser() =default;

    public:
        Tree::Document parse();

    public:
        // you can override this for your own stuff
        virtual void startElement(const Tree::Element&) {}
        virtual void endElement(const Tree::Element&) {}

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
        static bool namedReference(std::string_view, char32_t&);
        inline bool getNumericCharacterReference(char32_t&);
        inline bool getNamedCharacterReference(char32_t&);
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
        void addChild(Tree::Element&, std::unique_ptr<Tree::Node>);
        bool getNormalElementText(Tree::Element&);
        void getSpecialElementText(Tree::Element&, bool);
        static bool isVoidElement(std::string_view);
        void endTag(Tree::Element&);
        std::unique_ptr<Tree::Element> openElement();
        std::unique_ptr<Tree::Element> startTag(bool&);
        bool getElement(Tree::Element&);

    private:
        /* Text.hpp */
        std::unique_ptr<Tree::Text> getText();
    };
}

/************************************************************************/

#include "./Internal/CharClasses.hpp"
#include "./Internal/Conversions.hpp"
#include "./Internal/Tokens.hpp"
#include "./Internal/CharacterReference.hpp"
#include "./Internal/CharacterData.hpp"

#include "./Internal/Doctype.hpp"
#include "./Internal/Attribute.hpp"
#include "./Internal/Element.hpp"

/************************************************************************/

inline HTMLParser::Parser::Parser(std::string_view data)
    : buffer(data)
{
}

/************************************************************************/

inline HTMLParser::Tree::Document HTMLParser::Parser::parse()
{
    HTMLParser::Tree::Document document;
    skipCommentsAndSpace();
    skipDoctype();
    skipCommentsAndSpace();

    // This is rather hackish, but I wanted the Parser::startElement()
    // to get an element that already has the parent... so getElement()
    // needs a parent element.
    {
        Tree::Element root;
        getElement(root);
        assert(root.children.size()==1);

        Tree::Element* child=dynamic_cast<Tree::Element*>(root.children.front().release());
        assert(child!=nullptr);
        child->parent=nullptr;
        document.html=std::unique_ptr<Tree::Element>(child);
    }

    needs(document.html->name=="html");
    return document;
}

/************************************************************************/

#endif /* __cplusplus */
