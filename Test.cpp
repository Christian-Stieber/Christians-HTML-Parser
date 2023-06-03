#include <iostream>
#include <sstream>

#include "Headers/Parser.hpp"

/************************************************************************/
/*
 * https://stackoverflow.com/a/2602258
 */

int main()
{
    std::stringstream file;
    file << std::cin.rdbuf();
    HTMLParser::Parser parser(file.view());
    return 0;
}
