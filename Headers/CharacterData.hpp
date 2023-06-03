/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * endTag is only needed for non-normal types
 */

inline std::string HTMLParser::Parser::getCharacterData(HTMLParser::Parser::CharacterDataType type, std::string_view endTag)
{
    std::u32string data;
    while (true)
    {
        auto c=buffer.getChar();
        if (c=='&')
        {
            c=getCharacterReference();
        }
        else if (c=='<')
        {
            buffer.ungetChar();
            return UTF8Codecvt::convert(data);
        }
    }
}
