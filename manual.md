# Manual for cpphtmltags library

Homepage: https://github.com/skramm/cpphtmltags/


There is a single class to use: HTAG
It is defined in the namespace htags

A tag can be created in two different ways:

### Method 1
Just a regular object:
```C++
HTAG mytag( HT_P );
```
You can also add some content:
```C++
HTAG mytag( HT_P, "a paragraph" );
```
You can add a pair attribute/value:
```C++
HTAG mytag( HT_P, AT_CLASS, "abc" );
```
Or all three:
```C++
HTAG mytag( HT_P, "a paragraph", AT_CLASS, "abc" );
```
### Method 2
You can specify the file where the html code must be generated
(of type `std::ostream` or `std::ofstream`):
```C++
HTAG mytag( file, HT_P );
```

As above you can add a content, and/or a pair attribute/value, at creation time:
```C++
HTAG p1( file, HT_P, AT_CLASS, "abc" );
HTAG p2( file, HT_P, "a paragraph" );
HTAG p3( file, HT_P, "a paragraph", AT_CLASS, "abc" );
```

Please note that when created a tag that way, nothing is printed in the stream.
This will only happen when you call either
- `openTag()`
- `printTag()`
- `printWithContent( "something" )`

## Adding content to a tag

The "content" is what is inside the tag, i.e. `<p>Content</p>`

Content can be added in several ways.
- It can be added at the tag creation time (see above)
- It can be directly added to the output stream, once the tag is opened:
```C++
HTAG p( std::cout, HT_P );
p.openTag();
std::cout << "a paragraph";
p.closeTag();
```

- Or you can use the provided functions:
```C++
HTAG p( HT_P );
p.setContent( "a paragraph" );
p.addContent( " of text" );
```

## Global attributes

It is possible to add to a given tag a "global attribute", that is each time you will output that tag, this attribute-value pair will be automatically added.
For example, at one point you want to add to all the tags `li` the attribute `class="my_class"`.
This can be done with
`HTAG::addGlobalAttrib( HT_LI, AT_CLASS, "my_class" );`

To remove, you can:
-remove all global attributes with `HTAG::clearGlobalAttribs()`
-remove the global attribute on a given tag with `HTAG::clearGlobalAttrib( <tag id> )`


## Line feeds

In order to be human readable, it may be a good idea to have here and then some line feeds in the output html code.
On the other side, for large files it may be wanted to have "compact" html code, by removing all the unnecessary linefeeds.

These two behaviors can be achieved, three modes are available:
- a line feed after every closing of a tag;
- no line feed at all;
- a "default" mode, where certain tags get a line feed after closure, others do not.

This is achieved with the static method `setLineFeedMode( En_LineFeedMode mode )`.
The possible values are `LF_None`, `LF_Always`, `LF_Default`.

For example:
```C++
HTAG::setLineFeedMode( LF_Always );
```


## Error handling

In case of non fatal problem, this library will simply issue a warning with a clear message on `std::cerr`.
This can be disable by passing the option `HTAGS_DISABLE_WARNINGS` before including the file.

In case of fatal error, this library will issue a message on `std::cerr` with line number and the error message, and throw
an error of type `std::runtime_error`.

### html5 tag/attribute enforcing

The html5 standard specifies two kinds of attributes:
- "global" attributes: these can be assigned to any tag
- regular attributes: each of these can be assigned to a limited number of tags

This library can ensure that this is enforced by checking at each time you add an attribute to a tag.
This checking can be disabled by defining the symbol `HTTAGS_NO_CHECK` before the "include" line.

Copyright Sebastien Kramm - 2018

TO BE COMPLETED !
