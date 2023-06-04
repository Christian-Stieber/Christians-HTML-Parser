#include "Headers/Parser.hpp"

#include <iostream>
#include <sstream>

/************************************************************************/
/*
 * g++ -ggdb -std=c++20 Test.cpp
 */

/************************************************************************/

static void printAttribute(const HTMLParser::Tree::Element& element, std::string name)
{
    auto iterator=element.attributes.find(name);
    if (iterator!=element.attributes.end())
    {
        std::cout << " [" << iterator->first << "=" << iterator->second << "]";
    }
}

/************************************************************************/

static std::string shortText(const std::string& input)
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

static void printIndent(unsigned int indent)
{
    static const char indentString[]="   ";

    for (unsigned int i=0; i<indent; i++)
    {
        std::cout << indentString;
    }
}

/************************************************************************/

static void printNode(unsigned int indent, const HTMLParser::Tree::Node& node)
{
    auto element=dynamic_cast<const HTMLParser::Tree::Element*>(&node);
    if (element!=nullptr)
    {
        printIndent(indent);
        std::cout << "<" << element->name;
        printAttribute(*element, "id");
        printAttribute(*element, "class");
        std::cout << ">" << std::endl;
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
                std::cout << "\"" << output << "\"" << std::endl;
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

/************************************************************************/
/*
 * https://stackoverflow.com/a/2602258
 */

int main()
{
    std::stringstream file;
    file << std::cin.rdbuf();
    HTMLParser::Parser parser(file.view());
    printNode(0,*parser.document.html);
    return 0;
}
