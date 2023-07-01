#include "Headers/HTMLParser/Parser.hpp"
#include "Headers/HTMLParser/PrintTree.hpp"

#include <iostream>
#include <sstream>

/************************************************************************/
/*
 * g++ -ggdb -std=c++20 Test.cpp
 */

/************************************************************************/
/*
 * https://stackoverflow.com/a/2602258
 */

int main()
{
    std::stringstream file;
    file << std::cin.rdbuf();
    HTMLParser::Parser parser(file.view());
    std::cout << *parser.parse().html;
    return 0;
}
