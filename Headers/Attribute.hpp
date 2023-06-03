/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/
/*
 * Returns a pair name/value.
 *
 * If not attribute, name will be empty
 */

inline std::pair<std::string, std::string> HTMLParser::Parser::getAttribute()
{
    std::pair<std::string, std::string> result;
    std::string& name=result.first;
    std::string& value=result.second;
    name=getAttributeName();
    if (!name.empty())
    {
        auto hasValue=buffer.savePosition([this]() {
            skipWhitespace();
            return buffer.getChar()=='=';
        });
        if (hasValue)
        {
            skipWhitespace();
            switch(buffer.getChar())
            {
            case '"':
                value=getSomeString(&isDoubleQuotedAttributeValue, true);
                break;

            case '\'':
                value=getSomeString(&isSingleQuotedAttributeValue, true);
                break;

            default:
                buffer.ungetChar();
                value=getSomeString(&isUnquotedAttributeValue, true);
                needs(!value.empty());
                break;
            }
        }
    }
    return result;
}
