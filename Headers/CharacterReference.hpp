/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * Note: entity names are case-sensitive
 *
 * ToDo: this returns "REPLACEMENT CHARACTER" instead of throwing for
 * unknown names; this is because my table of names is woefully
 * incomplete.
 *
 * If we ever complete this table, we should reconsider this and
 * maybe throw SyntaxException. On the other hand, we're not
 * trying to be a validator, so maybe "REPLACEMENT CHARACTER"
 * is generally the way to go here...
 */

inline uint32_t HTMLParser::Parser::namedReference(std::string_view name)
{
    static const std::unordered_map<std::string_view, uint32_t> names={
        { "amp", '&' },
        { "lt", '<' },
        { "gt", '>' },
        { "quot", '"' },
        { "nbsp", 0x00a0 }
    };

    auto iterator=names.find(name);
    return (iterator==names.end()) ? 0xfffd : iterator->second;
}

/************************************************************************/
/*
 * Note: this assumes we've just read a '&' character.
 */

inline char32_t HTMLParser::Parser::getCharacterReference()
{
    uint32_t unicode=0;
    auto c=buffer.getChar();
    if (c=='#')
    {
        // ToDo: reject overflows?

        c=buffer.getChar();
        if (c=='x' || c=='X')
        {
            while (true)
            {
                c=toLower(buffer.getChar());
                if (c>='0' && c<='9')
                {
                    unicode=16*unicode+(c-'0');
                }
                else if (c>='a' && c<='f')
                {
                    unicode=16*unicode+(c-'a'+10);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            while (c>='0' && c<='9')
            {
                unicode=10*unicode+(c-'0');
                c=buffer.getChar();
            }
        }
        if (unicode==0x0000 || unicode==0x000d || (unicode>=0x0080 && unicode<=0x009f) || (unicode>=0xd800 && unicode<=0xdfff))
        {
            throw SyntaxException();
        }
    }
    else
    {
    }
    if (c!=';')
    {
        throw SyntaxException();
    }
    return static_cast<char32_t>(unicode);
}
