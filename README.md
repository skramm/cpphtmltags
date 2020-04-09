# cpphtmltags

[![Build Status](https://travis-ci.org/skramm/cpphtmltags.svg?branch=master)](https://travis-ci.org/skramm/cpphtmltags)


Generation of HTML content from C++ code

- Author: Sebastien Kramm (contact: firstname.lastname@univ-rouen.fr)
- Status: beta ! (2020/04: currently in development)
- Language: C++11
- Dependencies: none
- Homepage: https://github.com/skramm/cpphtmltags/
- Licence: LGPL v3
- Bugs/issues/requests: please use github issue tracker

## Goals

This library is designed to help any user code that needs to generate Html content from C++ code.
The goal is to make generation of valid HTML 5 content **simple** and **reliable**, with maximum versatility.
The idea is:
- to add some abstraction to HTML generation,
- to avoid unnecessary repetition of strings in your code,
- to avoid some basic errors that can happen if you directly generate the html code with hardcoded strings,
- to avoid generating invalid HTML 5 code.

For example, if you generate tags by some direct string output, some errors may go unnoticed.
Say you forget the 'p' in "<script>":<br>
`file << "<scrit>"`<br>
Well then the error won't get noticed until you carefully check the generated code.

Instead, by using this library, you will use something like:<br>
`file << Httag( HT_SCRIPT )`<br>
And in case you forget a letter somewhere, it will generate a compile-time error.

However, it is always possible to print out anything into the output stream, at any time (and even manually written Html tags!)

This library enforces wrong order of tags opening/closing, (aka "Misnested tags").
For example, something like this:<br>
`<h2>title<p></h2>paragraph</p>`<br>
may not happen.

This library also enforces the Html5 standard.
For example, some attributes are only allowed on certain tags.
This is enforced here, and if you attempt to use an illegal attribute, an error will be thrown at runtime.

The repo also holds several demo programs and a unit-test file.
Build and check these out with `make demo` (output programs are in `build/`) or `make test`.

## Usage

### Sample snippet 1:

```C++
#include "cpphtmltags.hpp"
int main()
{
	std::cout << Httag( HT_P ); // an empty paragraph

	httags::Httag t1( HT_P, AT_CLASS, "abc" );
	t1.addAttrib( AT_STYLE, "color:red;" );
	t1.setContent( "this is a paragraph" );
	std::cout << t1;
}
```
will produce:
```
<p></p><p class="abc" style="color:red;">this is a paragraph</p>
```

### Sample snippet 2:

```C++
int main()
{
	httags::Httag t1( HT_P, std::cout, AT_CLASS, "abc" );
	t1.setContent( "paragraph" );
	t1.printTag();
}
```
will produce:
```
<p class="abc">paragraph</p>
```

For more details, see [manual](docs/manual.md).


## HTML 5 coverage
- covers all Html5 tags ("elements") and all attributes (see page build/supported.html, generated with `make doc`)
- Comments (`<!-- ... -->`) and doctype are handled as special tags, see demo/demo0.cpp
- at present, the different tag categories are not considered, this is to be considered in future releases.
- GENERAL WARNING: while work was in progress, I discovered that the Html5 spec (see refs below) is a serious beast, in regard of its complexity.
For example, things like *this tag belongs to this category, but only in certain circumstances, which are defined in prose*.
These are numerous, making it hard to fetch a global picture.
This code is an attempt to tackle this complexity but (as usual) no warranty is given!


## Build and install
- build: no build, header-only (for details on how the code is generated [see here](docs/dev_info.md)).
- install: just fetch the single file [`cpphtmltags.hpp`](cpphtmltags.hpp), put it somewhere (`/usr/include` should be fine) and include it in your code.

## Related software
- C#: https://github.com/HtmlTags/htmltags
- C++:
  - https://github.com/SRombauts/HtmlBuilder/
 Main difference: this one seems to have one class per tag. Here the tags are handled through an enum.
  - https://github.com/tinfoilboy/CTML . This one uses strings to define tags, content and attributes, thus error prone.
  - http://ctpp.havoc.ru/en/ (seems dead)

## References

All the HTML 5 standard material has been fetched from these sources:
- general:
  - https://html.spec.whatwg.org/multipage/
  - https://www.w3.org/TR/html52/
- attributes:
  - https://www.w3.org/wiki/Html/Attributes/_Global
  - https://www.w3schools.com/tags/ref_attributes.asp
  - https://developer.mozilla.org/en-US/docs/Web/HTML/Attributes

- html syntax:
  - https://www.w3.org/TR/2011/WD-html-markup-20110113/syntax.html#syntax-elements

- Global attributes:
  - http://w3c.github.io/html-reference/global-attributes.html
  - https://www.w3schools.com/tags/ref_standardattributes.asp

- content categories:
 - https://www.w3.org/TR/html5/dom.html#kinds-of-content
 - https://www.w3.org/TR/html52/dom.html#kinds-of-content


Copyright Sebastien Kramm - 2018-2020
