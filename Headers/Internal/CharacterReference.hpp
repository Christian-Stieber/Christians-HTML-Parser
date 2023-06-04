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

inline bool HTMLParser::Parser::namedReference(std::string_view name, char32_t& result)
{
    /* ToDo: make this an array and use binary search */
    static const std::unordered_map<std::string_view, char32_t> names={
        { "amp", '&' },
        { "lt", '<' },
        { "gt", '>' },
        { "quot", '"' },
        { "nbsp", 0x00a0 }
    };

    auto iterator=names.find(name);
    if (iterator!=names.end())
    {
        result=iterator->second;
        return true;
    }
    return false;
}

/************************************************************************/
/*
 * ToDo: check for overflows?
 */

inline bool HTMLParser::Parser::getNumericCharacterReference(char32_t& result)
{
    bool success=false;
    uint32_t unicode=0;

    auto c=buffer.getChar();
    if (c=='#')
    {
        c=buffer.getChar();
        if (c=='x' || c=='X')
        {
            while (true)
            {
                c=toLower(buffer.getChar());
                if (c>='0' && c<='9')
                {
                    unicode=16*unicode+(c-'0');
                    success=true;
                }
                else if (c>='a' && c<='f')
                {
                    unicode=16*unicode+(c-'a'+10);
                    success=true;
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
                success=true;
                c=buffer.getChar();
            }
        }
    }
    else
    {
        buffer.ungetChar();
    }
    result=static_cast<char32_t>(unicode);
    return success;
}

/************************************************************************/

inline bool HTMLParser::Parser::getNamedCharacterReference(char32_t& result)
{
    auto name=getSomeString(&isCharacterReferenceName, false);
    return !name.empty() && namedReference(name, result);
}

/************************************************************************/
/*
 * Note: this assumes we've just read a '&' character.
 *
 * Note: I'm trying to be generous here. If what we're getting doesn't
 * look like a character reference, I'll just treat it as normal text
 * (which means I'm returning '&' and not advance the buffer).
 */

inline char32_t HTMLParser::Parser::getCharacterReference()
{
    char32_t result;

    bool success=buffer.savePosition([this, &result]() mutable {
        if (getNumericCharacterReference(result) || getNamedCharacterReference(result))
        {
            if (buffer.getChar()==';')
            {
                return true;
            }
        }
        return false;
    });

    return success ? result : '&';
}
