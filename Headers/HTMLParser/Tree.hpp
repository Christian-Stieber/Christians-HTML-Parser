/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

/************************************************************************/

namespace HTMLParser
{
    namespace Tree
    {
        class Element;

        class Node
        {
        public:
            Element* parent=nullptr;

        protected:
            Node() =default;

        public:
            virtual ~Node() =default;
        };
    }
}

/************************************************************************/
/*
 * Note: element and attribute names are in lowercase
 */

namespace HTMLParser
{
    namespace Tree
    {
        class Element : public Node
        {
        public:
            std::string name;
            std::vector<std::unique_ptr<Node>> children;

        public:
            // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
            struct AttributeNameHash : public std::hash<std::string_view>
            {
                typedef void is_transparent;
            };
            std::unordered_map<std::string, std::string, AttributeNameHash, std::equal_to<>> attributes;

        public:
            Element() =default;
            virtual ~Element() =default;

        public:
            const std::string* getAttribute(std::string_view) const;
        };
    }
}

/************************************************************************/

namespace HTMLParser
{
    namespace Tree
    {
        class Text : public Node
        {
        public:
            std::string text;

        public:
            Text(std::string text_)
                : text(std::move(text_))
            {
            }

            virtual ~Text() =default;
        };
    }
}

/************************************************************************/

namespace HTMLParser
{
    namespace Tree
    {
        class Document
        {
        public:
            std::unique_ptr<Element> html;
        };
    }
}

/************************************************************************/

inline const std::string* HTMLParser::Tree::Element::getAttribute(std::string_view name) const
{
    auto iterator=attributes.find(name);
    if (iterator!=attributes.end())
    {
        return &(iterator->second);
    }
    return nullptr;
}
