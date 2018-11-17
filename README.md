# cpphtmltags
Generation of HTML tags from C++ code

- Author: Sebastien Kramm (contact: firstname.lastname@univ-rouen.fr)
- Status: alpha
- Language: C++11

## Usage

### Sample snippet 1:

```C++
#include "cpphtmltags.hpp"
using namespace httags;

int main()
{
	HTAG t1( HT_P );
	std::cout << t1;

	t1.addAttrib( AT_CLASS, "aaa" );
	t1.addAttrib( AT_STYLE, "mystyle" );
	t1.setContent( "this is a paragraph" );
	std::cout << t1;
}
```
will produce:
`<p></p><p class="aaa" style="mystyle">this is a paragraph</p>`

### Sample snippet 2:


## Features


## Important points
- Does not intend (at present) to cover ALL html tags

## build and install
- build: no build, header-only
- install: just fetch the single file cpphtmltags.hpp, put it somewhere (/usr/include) and include it in your code.

## Related software
- C#: https://github.com/HtmlTags/htmltags
- C++:
 - https://github.com/SRombauts/HtmlBuilder/
 Main difference: this one seems to have one class per tag. Here the tags are handled through an enum.
 - https://github.com/tinfoilboy/CTML

## Motivation
Needed this, couldn't find...
