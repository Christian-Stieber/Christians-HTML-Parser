/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include "./Tree.hpp"

#include <ostream>

/************************************************************************/

namespace HTMLParser
{
    class PrintTree
    {
    private:
        std::ostream& stream;

    private:
        void printAttribute(const HTMLParser::Tree::Element&, std::string);
        static std::string shortText(const std::string&);
        void printIndent(unsigned int);
        void printNode(unsigned int, const HTMLParser::Tree::Node&);

    public:
        PrintTree(std::ostream& stream_)
            : stream(stream_)
        {
        }

    public:
        void print(const HTMLParser::Tree::Node& node)
        {
            printNode(0, node);
        }
    };
}

/************************************************************************/

inline std::ostream& operator<<(std::ostream& stream, const HTMLParser::Tree::Node& node)
{
    HTMLParser::PrintTree(stream).print(node);
    return stream;
}

/************************************************************************/

inline void HTMLParser::PrintTree::printAttribute(const HTMLParser::Tree::Element& element, std::string name)
{
    auto iterator=element.attributes.find(name);
    if (iterator!=element.attributes.end())
    {
        stream << " [" << iterator->first << "=" << iterator->second << "]";
    }
}

/************************************************************************/

inline std::string HTMLParser::PrintTree::shortText(const std::string& input)
{
    std::string output;
    bool addSpace=false;
    bool truncate=false;

    for (char c : input)
    {
        if (c==0x0a || c==0x0d || c==0x09)
        {
            c=' ';
        }
        if (c==' ')
        {
            if (!output.empty())
            {
                addSpace=true;
            }
        }
        else
        {
            if (truncate)
            {
                output.append("…");
                break;
            }
            if (addSpace)
            {
                addSpace=false;
                output.push_back(' ');
            }
            output.push_back(c);
            if (output.size()>=80)
            {
                truncate=true;
            }
        }
    }

    return output;
}

/************************************************************************/

inline void HTMLParser::PrintTree::printIndent(unsigned int indent)
{
    static const char indentString[]="   ";

    for (unsigned int i=0; i<indent; i++)
    {
        stream << indentString;
    }
}

/************************************************************************/

inline void HTMLParser::PrintTree::printNode(unsigned int indent, const HTMLParser::Tree::Node& node)
{
    auto element=dynamic_cast<const HTMLParser::Tree::Element*>(&node);
    if (element!=nullptr)
    {
        printIndent(indent);
        stream << "<" << element->name;
        printAttribute(*element, "id");
        printAttribute(*element, "class");
        stream << ">\n";
    }
    else
    {
        auto text=dynamic_cast<const HTMLParser::Tree::Text*>(&node);
        if (text!=nullptr)
        {
            auto output=shortText(text->text);
            if (!output.empty())
            {
                printIndent(indent);
                stream << "\"" << output << "\"\n";
            }
        }
    }

    if (element!=nullptr)
    {
        for (const auto& child : element->children)
        {
            printNode(indent+1, *child);
        }
    }
}
