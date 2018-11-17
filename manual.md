# Manual for cpphtmltags library

Homepage: https://github.com/skramm/cpphtmltags/


There is a single class to use: HTAG
It is defined in the namespace htags

A tag can be created in two different ways:
- just a regular object, that you can stream
- an object holding a file descriptor, that must exist and be opened.


## Adding content to a tag

The "content" is what is inside the tag, i.e. "<p>Content</p>"

Once the tag is created, you can st the content in two ways:
- either by assigning it (this will erase previous content):<br>
`tag.setContent( "this is my content" );`
- or by streaming stuff into it:<br>
`tag << "the number is " << 42;`

The first method can only be used only for strings and other POD types (int, floats, ...).




## Line feeds

In order to be human readable, it may be a good idea to have here and then some line feeds in the output html code.
On the other side, for large files its may be wanted to have "compact" html code, by removing all the unnecessary linefeeds.

These two behaviors can be achieved, three modes are actually available:
- a line feed after every closing of a tag;
- no line feed at all;
- a "default" mode, where certain tags get a line feed after closure, others do not.

This is achieved with the static method `setLineFeedMode( En_LineFeedMode mode )`.
The possible values are LF_None, LF_Always, LF_Default.
For example:
```C++
HTAG::setLineFeedMode( LF_Always );
```


## Error handling

TO BE COMPLETED !
