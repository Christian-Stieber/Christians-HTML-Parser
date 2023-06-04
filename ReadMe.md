# GOALS

This is a simple HTML parser in C++.

Assumes C++20, mostly because that's what I currently
use. Header-only, no external dependencies.

The main purpose is to let me get the data from certain Steam
webpages, so that's also my main "testing".

I still made this a separate project, so I can maybe use it in other
projects as well. Thus, I'm somewhat interested in fixing problems
with other pages... if it's not too much work.

# STANDARDS COMPLIANCE

* "what's that?"
* it reads some semblance of HTML
* I've tried following a W3C document that I found, but it seems
  I didn't read it properly
* there's been trial-and-error to get webpages to parse

# KNOWN SHORTCOMINGS

* doesn't handle "tag soup" and other invalid markup; this might be complicated to do
* the list of named character references is woefully incomplete
* no information on errors
* assumes UTF-8 input, and creates UTF-8 output

# STUFF THAT IT DOESN'T WANT TO DO

* no query functions -- just walk through the tree yourself
* probably not a benchmark winner
