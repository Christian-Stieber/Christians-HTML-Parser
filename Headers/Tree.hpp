/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

/************************************************************************/

namespace HTMLParser
{
    namespace Tree
    {
        class Node
        {
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
            std::unordered_map<std::string, std::string> attributes;

        public:
            Element() =default;
            virtual ~Element() =default;
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
