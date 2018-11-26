# cpphtmltags
Generation of HTML tags from C++ code

- Author: Sebastien Kramm (contact: firstname.lastname@univ-rouen.fr)
- Status: beta
- Language: C++11
- Dependencies: none
- Homepage: https://github.com/skramm/cpphtmltags/
- Licence: LGPL v3
- Bugs/issues/requests: please use github issue tracker

## Goals

This library is designed to help any user code that needs to generate Html content from C++ code.
The goal is to make generation of valid html5 content from C++ code **simple** and **reliable**, with maximum versatility.
The idea is:
- to add some abstraction to html generation,
- to avoid unnecessary repetition of strings in your code,
- to avoid some basic errors that can happen if you directly generate the html code with hardcoded strings?
- to avoid generating invalid html5 code.

For example, if you generate tags by some direct string output,
some errors may go unnoticed. Say you forget the 'p' in "script":<br>
`file << "<scrit>"`<br>
Well then the error won't get noticed until you carefully check the generated code.

Instead, by using this library, you will use something like:<br>
`file << Httag( HT_SCRIPT )`<br>
And in case you forget a letter somewhere, it will generate a compile-time error.

However, it is always possible to print out anything into the output stream, at any time (and even manually written Html tags!)

This library enforces wrong order of tags opening/closing, for example, something like this:<br>
`<h2>title<p></h2>paragraph</p>`<br>
may not happen.

This library also enforces the html5 standard.
For example, you known that some attributes are only allowed on certain tags.
This is considered here, and if you attempt to use an illegal attribute, an error will be thrown at runtime.

## Usage

### Sample snippet 1:

```C++
#include "cpphtmltags.hpp"
using namespace httags;

int main()
{
	std::cout << Httag( HT_P ); // an empty paragraph

	Httag t1( HT_P, AT_CLASS, "abc" );
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
	Httag t1( HT_P, std::cout, AT_CLASS, "abc" );
	t1.setContent( "paragraph" );
	t1.printTag();
}
```
will produce:
```
<p class="abc">paragraph</p>`
```

For more details, see [manual](manual.md).

## Html coverage
- covers 123 html tags ("elements", as they say at w3c) and 134 attributes
- limitations: html comments not handled at present

## Build and install
- build: no build, header-only
- install: just fetch the single file (`cpphtmltags.hpp`)[cpphtmltags.hpp], put it somewhere (`/usr/include` should be fine) and include it in your code.

## Motivation
Needed this, couldn't find...

## Related software
- C#: https://github.com/HtmlTags/htmltags
- C++:
  - https://github.com/SRombauts/HtmlBuilder/
 Main difference: this one seems to have one class per tag. Here the tags are handled through an enum.
  - https://github.com/tinfoilboy/CTML

## References

All the html5 standard material has been fetched from these sources:
- attributes:
  - https://www.w3.org/wiki/Html/Attributes/_Global
  - https://www.w3schools.com/tags/ref_attributes.asp
  - https://developer.mozilla.org/en-US/docs/Web/HTML/Attributes

- html syntax:
  - https://www.w3.org/TR/2011/WD-html-markup-20110113/syntax.html#syntax-elements

- Global attributes:
  - http://w3c.github.io/html-reference/global-attributes.html
  - https://www.w3schools.com/tags/ref_standardattributes.asp

Copyright Sebastien Kramm - 2018
