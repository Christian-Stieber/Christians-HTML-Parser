# GOALS

This is a simple HTML parser in C++.

Assumes C++20, mostly because that's what I currently use. Header-only, no external dependencies.

The main purpose is to let me get the data from certain Steam webpages, so that's also my main "testing".

I still made this a separate project, so I can maybe use it in other projects as well. Thus, I'm somewhat interested in fixing problems with other pages... if it's not too much work.

# STANDARDS COMPLIANCE

None. It works with some pages, and fails for others.

# KNOWN SHORTCOMINGS

* No CDATA... yet
* doesn't handle "tag soup" and other invalid markup; this might be complicated to do
* the list of named character references is woefully incomplete
* no detail information on errors
* assumes UTF-8 input, and creates UTF-8 output

# STUFF THAT IT DOESN'T WANT TO DO

* no query functions -- just walk through the tree yourself, or use the callbacks
* probably not a benchmark winner

# USAGE

If you have an UTF-8 encoded webpage, parse it like this:

```
#include "HTMLParser/Parser.hpp"

void example()
{
    std::string html="…";
    ...
    HTMLParser::Parser parser(html);
    HTMLParser::Tree::Document document=parser.parse();
}
```

Refer to the `HTMLParser/Tree.hpp` file for the document data; it should be rather straightforward.

You can also subclass the `Parser` class to learn about elements when they are created:

```
class MyParser : public HTMLParser::Parser
{
   using Parser::Parser;

public:
   virtual ~MyParser() =default;

   virtual void startElement(const HTMLParser::Tree::Element& element) override
   {
   }

   virtual void endElement(const HTMLParser::Tree::Element& element) override
   {
   }
};
```

This lets you build lookup structures as elements are created (of course, you can just walk through the tree afterwards instead).

`startElement` is called right after reading the start tag. Your element will have a `parent`, but children will not be available.

`endElement` is called after reading the end tag. This means the children list will be completed as well.

Note: for both calls, even the `html` element at the top of the document will have a `parent` pointer, which is pointing to an element with no name. This is a parsing artifact; it will not appear in the final document tree.
