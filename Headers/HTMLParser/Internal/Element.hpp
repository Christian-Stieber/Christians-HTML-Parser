/*
 * Copyright (C) 2023- Christian Stieber
 */

/************************************************************************/

inline bool HTMLParser::Parser::isVoidElement(std::string_view tag)
{
    static constexpr std::string_view voidTags[16]={
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

inline void HTMLParser::Parser::addChild(HTMLParser::Tree::Element& parent, std::unique_ptr<HTMLParser::Tree::Node> child)
{
    assert(child->parent==nullptr);
    child->parent=&parent;
    parent.children.push_back(std::move(child));
}

/************************************************************************/

inline bool HTMLParser::Parser::getNormalElementText(HTMLParser::Tree::Element& parent)
{
    std::string text=getNormalCharacterData();
    if (!text.empty())
    {
        auto child=std::make_unique<HTMLParser::Tree::Text>(std::move(text));
        addChild(parent, std::move(child));
        return true;
    }
    return false;
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
    addChild(element, std::move(text));
}

/************************************************************************/

inline bool HTMLParser::Parser::getElement(HTMLParser::Tree::Element& parent)
{
    bool closed=false;
    auto elementPtr=startTag(closed);
    if (elementPtr)
    {
        auto& element=*elementPtr;
        addChild(parent, std::move(elementPtr));
        startElement(element);
        if (!closed && !isVoidElement(element.name))
        {
            std::string_view name{element.name};
            if (name=="script" || name=="style")
            {
                getSpecialElementText(element, false);
            }
            else if (name=="title" || name=="textarea")
            {
                getSpecialElementText(element, true);
            }
            else
            {
                while (skipComment() || getElement(element) || getNormalElementText(element))
                    ;
            }
            endTag(element);
        }
        endElement(element);
        return true;
    }
    return false;
}
