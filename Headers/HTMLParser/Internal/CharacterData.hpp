/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

inline std::string HTMLParser::Parser::getNormalCharacterData()
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
        data.push_back(c);
    }
}

/************************************************************************/
/*
 * allowCharacterReferences true -> replaceable character data
 * allowCharacterReferences false -> non-replaceable character data
 */

inline std::string HTMLParser::Parser::getSpecialCharacterData(bool allowCharacterReferences, const char* endTag)
{
    std::u32string data;
    while (true)
    {
        auto result=buffer.savePosition([this, endTag]() {
            if (skipString("</") && skipString(endTag))
            {
                auto c=buffer.getChar();
                if (c=='>' || c=='/' || isWhitespace(c))
                {
                    return false;
                }
            }
            return true;
        });

        if (!result)
        {
            return UTF8Codecvt::convert(data);
        }

        auto c=buffer.getChar();
        if (allowCharacterReferences && c=='&')
        {
            c=getCharacterReference();
        }
        data.push_back(c);
    }
}
