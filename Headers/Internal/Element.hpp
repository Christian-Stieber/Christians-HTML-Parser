/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

inline bool HTMLParser::Parser::isVoidElement(std::string_view tag)
{
    static constexpr std::string_view voidTags[]={
        /* make sure these are sorted */
        "area", "base", "br", "col", "command", "embed",
        "hr", "img", "input", "keygen", "link", "meta",
        "param", "source", "track", "wbr"
    };

    // Note: we could use std::binary_search; I'm just
    // not sure whether we migth need a few more objects
    // in the list for this to be better.

    for (const auto& voidTag : voidTags)
    {
        auto result=(voidTag<=>tag);
        if (result==0) return true;
        if (result>0) return false;
    }
    return false;
}

/************************************************************************/
/*
 * A "<" character.
 * The element’s tag name.
 *
 * Returns a newly allocated Element, or a nullptr.
 */

inline std::unique_ptr<HTMLParser::Tree::Element> HTMLParser::Parser::openElement()
{
    std::unique_ptr<HTMLParser::Tree::Element> element;

    buffer.savePosition([this, &element]() mutable {
        if (buffer.getChar()=='<')
        {
            auto tagname=getTagname();
            if (!tagname.empty())
            {
                element=std::make_unique<HTMLParser::Tree::Element>();
                element->name=std::move(tagname);
                return true;
            }
        }
        return false;
    });

    return element;
}

/************************************************************************/
/*
 * A "<" character.
 * The element’s tag name.
 * Optionally, one or more attributes, each of which must be preceded by one or more space characters.
 * Optionally, one or more space characters.
 * Optionally, a "/" character, which may be present only if the element is a void element.
 * A ">" character.
 *
 * Returns a newly allocated Element, or a nullptr if not an element. Throws on errors.
 */

inline std::unique_ptr<HTMLParser::Tree::Element> HTMLParser::Parser::startTag(bool& closed)
{
    auto element=openElement();
    if (element)
    {
        while (skipWhitespace())
        {
            auto attribute=getAttribute();
            if (attribute.first.empty())
            {
                break;
            }
            needs(element->attributes.insert(std::move(attribute)).second);
        }

        /*
         * Note: my impression from the HTML description was that self-closing
         * tags aren't allowed except for void elements. But, I've found them
         * in real pages, so I'll just allow them everywhere.
         */
        if (skipString("/"))
        {
            closed=true;
        }

        needs(skipString(">"));
    }
    return element;
}

/************************************************************************/

inline std::unique_ptr<HTMLParser::Tree::Text> HTMLParser::Parser::getNormalElementText()
{
    std::unique_ptr<HTMLParser::Tree::Text> result;
    std::string text=getNormalCharacterData();
    if (!text.empty())
    {
        result=std::make_unique<HTMLParser::Tree::Text>(std::move(text));
    }
    return result;
}

/************************************************************************/

inline void HTMLParser::Parser::elementContent(HTMLParser::Tree::Element& element)
{
    while (true)
    {
        std::unique_ptr<HTMLParser::Tree::Node> child;

        if (skipComment())
        {
        }
        else if ((child=getElement()) || (child=getNormalElementText()))
        {
            element.children.push_back(std::move(child));
        }
        else
        {
            break;
        }
    }
}

/************************************************************************/
/*
 * A "<" character.
 * A "/" character
 * The element’s tag name.
 * Optionally, one or more space characters.
 * A ">" character.
 */

inline void HTMLParser::Parser::endTag(HTMLParser::Tree::Element& element)
{
    needs(skipString("</") &&
          skipString(element.name.c_str()) &&
          (skipWhitespace(), true) &&
          skipString(">"));
}

/************************************************************************/

inline void HTMLParser::Parser::getSpecialElementText(HTMLParser::Tree::Element& element, bool allowCharacterReferences)
{
    std::string data=getSpecialCharacterData(allowCharacterReferences, element.name.c_str());
    auto text=std::make_unique<HTMLParser::Tree::Text>(std::move(data));
    element.children.push_back(std::move(text));
}

/************************************************************************/

inline std::unique_ptr<HTMLParser::Tree::Element> HTMLParser::Parser::getElement()
{
    bool closed=false;
    auto element=startTag(closed);
    if (element)
    {
        if (!closed && !isVoidElement(element->name))
        {
            std::string_view name{element->name};
            if (name=="script" || name=="style")
            {
                getSpecialElementText(*element, false);
            }
            else if (name=="title" || name=="textarea")
            {
                getSpecialElementText(*element, true);
            }
            else
            {
                elementContent(*element);
            }
            endTag(*element);
        }
    }
    return element;
}
