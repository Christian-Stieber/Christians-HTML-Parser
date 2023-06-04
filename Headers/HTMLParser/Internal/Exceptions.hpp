/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

namespace HTMLParser
{
    class EOFException { };
    class ConversionException { };
    class SyntaxException { };

    void needs(bool result)
    {
        if (!result) throw SyntaxException();
    }
}
