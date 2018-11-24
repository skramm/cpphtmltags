# cpphtmltags
Generation of HTML tags from C++ code

- Author: Sebastien Kramm (contact: firstname.lastname@univ-rouen.fr)
- Status: alpha
- Language: C++11
- homepage: https://github.com/skramm/cpphtmltags/
- licence: LGPL v3

## Goals

The goal is to make generation of valid html5 content from C++ code easy and reliable, with maximum versatility.
The idea is:
- to add some abstraction to html generation,
- to avoid unnecessary repetition of strings in your code,
- to avoid some basic errors that can happen if you directly generate the html code with hardcoded strings.

For example, if you generate tags by some direct string output,
some errors may go unnoticed. Say you forget the 'p' in "script":
 `file << "<scrit>"`.
Well then the error won't get noticed until you carefully check the generated code.

Instead, by using this library, you will use something like:
`file << HTAG( HT_SCRIPT )`.
And in case you forget a letter somewhere, it will generate a compile-time error.


## Usage

### Sample snippet 1:

```C++
#include "cpphtmltags.hpp"
using namespace httags;

int main()
{
	HTAG t1( HT_P );
	std::cout << t1;

	t1.addAttrib( AT_CLASS, "abc" );
	t1.addAttrib( AT_STYLE, "color:red;" );
	t1.setContent( "this is a paragraph" );
	std::cout << t1;
}
```
will produce:
`<p></p><p class="abc" style="color:red;">this is a paragraph</p>`

### Sample snippet 2:


For more details, see [manual](manual.md).

## Features


## Important points
- Does not intend (at present) to cover ALL html tags

## build and install
- build: no build, header-only
- install: just fetch the single file cpphtmltags.hpp, put it somewhere (/usr/include) and include it in your code.

## Motivation
Needed this, couldn't find...

## Related software
- C#: https://github.com/HtmlTags/htmltags
- C++:
  - https://github.com/SRombauts/HtmlBuilder/
 Main difference: this one seems to have one class per tag. Here the tags are handled through an enum.
  - https://github.com/tinfoilboy/CTML

