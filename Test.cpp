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

static void printElement(unsigned int indent, const HTMLParser::Tree::Element& element)
{
    for (unsigned int i=0; i<indent; i++)
    {
        std::cout << "   ";
    }
    std::cout << element.name;
    printAttribute(element, "id");
    printAttribute(element, "class");
    std::cout << std::endl;

    for (const auto& child : element.children)
    {
        auto childElement=dynamic_cast<const HTMLParser::Tree::Element*>(child.get());
        if (childElement!=nullptr)
        {
            printElement(indent+1, *childElement);
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
    printElement(0,*parser.document.html);
    return 0;
}
