# Developper information

Here, some details about the project for the ones that may be interested in contributing.
For more details, also run `make doc` in the root folder (needs Doxygen).

## Generation of library file

Starting from release 1.0, the actual library file (`cpphtmltags.hpp`) is actually generated from several files.
The main C++ code is in 3 source files: `cpphtmltags_1.hh`, `cpphtmltags_2.hh` and `cpphtmltags_3.hh`.
The final file is build according to this diagram:

<table>
<tr><td><code>cpphtmltags_1.hh</code></td></tr>
<tr><td>generated content</td></tr>
<tr><td><code>cpphtmltags_2.hh</code></td></tr>
<tr><td>generated content</td></tr>
<tr><td><code>cpphtmltags_3.hh</code></td></tr>
</table>

The generated content is build by the bash scripts `build.sh` (and 2 others).
It takes as input the html reference material that is located in the files located in the `ref` subfolder:
- `global_attribs.ref` holds the list of global attributes (the one that can be used with any html element).
- `valid_attribs.ref` holds the list of authorized tags for a given attribute, in the form `attrib:tag1,tag2,tag3`.
It is used along with `global_attribs.ref` to generate a file holding all the attributes, that is used to generate the code.
- `void_elements.ref` holds the list of void-elements (tags that don't get closed)
- `tag_content.ref` :processed by script `bcontent.sh`, will generate the class httag::priv::AllowedContentMap
- `element_cat.ref` :processed by script `bcat.sh`, will generate

See <a href="../supported.html">supported tags</a> (page available only on locally-build doc pages).

Four steps:
- build the reference file holding all the attributes
- build tags and attributes enum and `getString()` functions from plain text files
- build c++ code to initialize a map holding for each attribute the tags where they are allowed.
- concatenate first part of header, this generated code, and final part of header to generate the main header file.

The advantage of such an approach is that in case new tags or attributes are integrated into the standard, only these files need to be edited.


## Static variables

All the reference stuff (tags/attribs strings, map of allowed attributes, ...) are stored in static variables.
In order to make this work in a header file, the classical "lazy static initialization" trick is used:
The variable is stored in a static function that returns a reference on the variable.
The variable is build upon first call of the function.


## Testing

This project has an automated unit test file, based on the [catch](https://github.com/catchorg/Catch2/) framework.
If is is installed on your machine, you can run the tests with `make test`.
Currently used with catch 2.3.0.
Coverage is currently not complete, but will expand.


## Misc. design choices

### Tag categories and types

The HTML5 standard mandates that some tags can only hold as content a limited set of tags.
To factorize things, it defines tag categories.
The main ones are defined here:
https://html.spec.whatwg.org/multipage/dom.html#kinds-of-content

- The file ref/element_cat.ref defines for each category the list of tags that belong to that category.
- The file ref/tag_content.ref defines for each tag what tag and/or category it accepts as content.

The list of categories is build automatically by `bcat.sh` from file `ref/element_cat.ref`.
The script will generate the enum `httag::priv::En_TagCat`
These categories must match the one given to describe the category a tag belongs to, in file `ref/tag_content.ref`

In some situations, the allowed content is described as:
> "If the element has a XXXX attribute: zero or more YYYY elements [...]. If the element does not have a XXXX attribute:  zero or more ZZZZ elements [...]"

In such a situation, both `YYYY` and `ZZZZ` are allowed, no matter the presence of the attribute.




#### "Empty" vs. "void" elements

We consider here these are the same.
For a bit of context, see https://stackoverflow.com/a/25314415/193789

See list here: https://developer.mozilla.org/en-US/docs/Glossary/Empty_element


## Coding conventions

- TABS for indentation, SPC for... spacing
- private member functions are prefixed with `p_`
- static variables are prefixed with `s_`
- all enums are prefixed with `En_`
- all enum values belonging to same enum are prefixed with same uppercase prefix ('XX_')


## Handling ot authorized content

This is done through a map that hold for each tag a value of type httag::priv::AllowedContent
It is wrapped in the type httag::priv::AllowedContentMap.
These two types are dymalically generated at build time.

The type httag::priv::AllowedContent holds for a given tag all the allowed content for that tag, as:
- a list of allowed tags
- a list of allowed categories of tags (see httag::priv::En_TagCat)
- a list of non-allowed tags
- a list of non-allowed categories of tags



## Other stuff, todos, ...

\todo checkout this:
https://www.boost.org/doc/libs/1_72_0/doc/html/stacktrace/getting_started.html
Maybe it can be used to access the caller code in case of error?


\todo 20200326: maybe merge together printWithContent() and printTag()

\todo 20200327: put back the silent error mode (see HTTAG_NO_CHECK)

\todo 20200329: check bevavior of f << tag: does tag get cleared ?

