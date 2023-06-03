/*
 * Copyright (C) 2023- Christian Stieber
 */

#pragma once

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
