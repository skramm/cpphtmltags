# Manual for cpphtmltags library

Homepage: https://github.com/skramm/cpphtmltags/

## Introduction
This library is build around a single class: `Httag`, along with some helper free functions.
All of it is defined in the namespace `httag`.
This class models an HTML tag (aka element), that can be streamed in file.

Two types of tags can be created, but they are both handled trough the same class, for conveniency.
They only differ in the way they are created, and how they generate some output.
- The first type is independent of any output device, it is up to the user to stream them in some output stream.
- With the second type, you assign an output stream at creation time.

The library and the contained class can be used in two ways, either through classical object paradigm, either through a set of macros.
These basically wrap the code, it ends up as the same thing, with one notable exception:
All the potential errors you might do while writing your code (say, trying to write something like `<p><p>abc</p></p>`, which is illegal) will throw an error.
- With the classical API, your program will stop (unless you try to catch them, but that would be tedious).
- With the macro-based API, your program will continue, and you will get in stderr a nice information log, with the location of the error in your code.
For more details, [see here](#macro).

But anyhow, you might want to check the classical API first.


## A - Classical User Interface


### A.1 - Instanciation

Two types of constructors are available, corresponding to the two types of objects.

#### A.1.a - Type 1 constructor
Just a regular object:
```C++
Httag mytag( HT_P );
```
You can also add some content:
```C++
Httag mytag( HT_P, "a paragraph" );
```
You can add a pair attribute/value:
```C++
Httag mytag( HT_P, AT_CLASS, "abc" );
```
Or both, at creation time:
```C++
Httag mytag( HT_TD, "my cell", AT_COLSPAN, 2 );
```

#### A.1.b - Type 2 constructor ("file-type" object)

You can specify the stream where the html code must be generated.
It can be of type `std::ostream`, or `std::ofstream`, or even `std::ostringstream`.
`std::cout` works fine too:
```C++
Httag mytag( file, HT_P );
```

As above you can add a content, and/or a pair attribute/value, at creation time:
```C++
Httag p1( file, HT_P, AT_CLASS, "abc" );
Httag p2( file, HT_P, "a paragraph" );
Httag p3( file, HT_P, "a paragraph", AT_CLASS, "abc" );
```
Please note that when a tag is created that way, nothing is printed in the stream.
This will only happen when you call either one of these on the variable:
- `openTag()` : only prints the opening tag
- `printTag()`: prints the whole thing, opening tag, content, and closing tag
- `printWithContent( "something" )`

Once it is opened, you can explicitly close it with `closeTag()`.
But this also called automatically by the destructor, which is called when the variable goes out of scope.

#### A.1.c - Which one should I use ?

Theorically, both could achieve the same html structure.

The first type of tags (that has no associated stream/file) can be used in a hierarchical way:
you can stream into it a `Httag` variable, either an explicit one, or an anonymous one, through a constructor call.
For example, a minimal page holding 2 divs, one empty and the second one holding a paragraph could be written this way:
```
Httag html( HT_HTML );
Httag head( HT_HEAD );
Httag body( HT_BODY );
head << Httag( HT_TITLE, "my title" );
body << Httag( HT_DIV, AT_ID, "div1" );  // an empty div
Httag div2( HT_DIV, AT_ID, "div2" );
div2 << Httag( HT_P, "Some text" );
body << div2;
html << head << body;
std::cout << html;
```

However, it is often easier to use the "file-type" tag so one can explicitly open it before adding the rest of the page.

Shortly, it depends on the context, this library offers you maximum flexibility.


### A.2 - Adding text content to a tag

The "content" is what is inside the tag, i.e. `<p>Content</p>`

Content can be added in several ways.
- It can be added at the tag creation time (see above)
- It can be directly added to the output stream, once the tag is opened:
```C++
Httag p( std::cout, HT_P );
p.openTag();
std::cout << "a paragraph";
p.closeTag();
```
If `p` is not used afterwards, it would be faster to write:
```C++
std::cout << Httag( HT_P, "a paragraph" );
```


- Or you can use the provided functions (can be used with the two types of tags):
```C++
Httag p( HT_P );
p.setContent( "a paragraph" );
p.addContent( " of text" );
std::cout << p;  // <p>a paragraph of text</p>
```

These function can be used in a chained way.
For example:
```C++
std::string hello{"hello"};
Httag p( HT_P );
p.setContent( "say " ).addContent( hello );
std::cout << p;  // <p>say hello</p>
```
Or, as a one-liner:
```C++
f << Httag ( HT_P ).setContent( "say " ).addContent( hello );
```
- But you can also directly stream something **into** the tag.
This will produce the same as above:
```C++
Httag p( HT_P );
p << "a paragraph";
p << " of text";
std::cout << p;
```

But there is more: the content can be another tag!
You can stream a tag inside a tag, or use it as a content.
For example:
```C++
Httag a( HT_A, "a link", AT_HREF, "https://somewhere.com" );
Httag li( HT_LI, a );
std::cout << li;
```
will produce:<br>
`<li><a href="https://somewhere.com">a link</a></li>`

Another way of achieving this would be:
```C++
Httag li( HT_LI );
li << Httag( HT_A, "a link", AT_HREF, "https://somewhere.com" );
std::cout << li;
```

Even this is possible! :
```C++
Httag t( HT_DIV, Httag( HT_DIV, "aaa" ) );
std::cout << t;
```
and will produce: `<div><div>aaa</div></div>`

In some situation, you could want to insert as content a tag with its content AND a string.
For example<br>
`<p>This is <strong>bold text</strong></p>`<br>
Then, you need to transform the Httag object into a string.
A method `to_string()` is provided, so you can write:
```C++
Httag p( HT_P, "This is " + Httag( HT_STRONG, "bold text" ).to_string() );
std::cout << t;
```
But it is in fact unnecessary:
an implicit conversion to std::string is provided, so you can also write:
```C++
Httag p( HT_P, "This is " + Httag( HT_STRONG, "bold text" ) );
std::cout << t;
```



### A.3 - Attributes

#### A.3.1 - Adding attributes
Tag attributes can be added to the tag at creation time (see above) or afterwards:
```C++
Httag p( HT_P );
p.addAttrib( AT_CLASS, "abc" );
```

The API is templatized, so for some attributes, you can use integer values as well:
```C++
Httag td( HT_TD );
td.addAttrib( AT_COLSPAN, 3 );
```

Beware, for 'file-type' tags, you can add attributes but only before opening the tag:
```C++
Httag td( std::cout, HT_TD );
td.addAttrib( AT_COLSPAN, 3 );  // fine
td.openTag();
td.addAttrib( AT_CLASS, "abc" );  // this will throw an error
```

A specific attribute can be removed from a variable, for example:
```C++
td.removeAttrib( AT_COLSPAN );
```
This can also be chained with other member functions:
```C++
td.removeAttrib( AT_COLSPAN ).addAttrib( AT_ID, "myid" );
td.addAttrib( AT_ID, "myid" ).removeAttrib( AT_COLSPAN ); //does the same as above !
```

You can clear all attributes with:
```C++
td.clearAttribs();
```

#### A.3.2 - Boolean attributes

These are attributes that have no value.
For example the tag `<input>` allows the boolean attribute `checked`.
So you can write
```C++
f << Httag( HT_INPUT ).addAttrib( AT_CHECKED);
```
which will produce: `<input checked>`.

The only difference is that, for technical considerations due to the way C++ does the template resolution, you cannot add a boolean attribute with the constructor.
This:<br>`f << Httag( HT_INPUT, AT_CHECKED );`<br>will fail.
But you can use an empty string value.
This:<br>
`f << Httag( HT_INPUT, AT_CHECKED, "" );`<br>
is fine.

<a name="global_attribute"></a>
#### A.3.3 - Tag global attributes

It is possible to add to a given tag a "global attribute", that is each time you will output that tag, this attribute-value pair will be automatically added.
For example, at one point you want to add to all the tags `li` the attribute `class="my_class"`.
This can be done with
`Httag::setGlobalAttrib( HT_LI, AT_CLASS, "my_class" );`

To remove, you can:
- remove all global attributes:<br>
`Httag::clearGlobalAttribs()`
- remove the global attribute on a given tag:<br>
`Httag::clearGlobalAttrib( <tag id> )`<br>
For example:
`Httag::clearGlobalAttrib( HT_LI )`

An example of usefulness of this features is for the `<a>` tag.
As you know, you can add the attribute `target="_blank"` to make the link open in another tab/window of the browser.
So if you want all of your generated links to have automatically this feature, just add this in you code:
```C++
Httag::setGlobalAttrib( HT_A, AT_TARGET, "_blank" );
```

All these are static function, thus the `Httag::` prefix.

#### A.3.4 - Attributes uniqueness enforcement

HTML 5 mandates that no attribute shall be present more than once.
This is enforced here, and adding attributes will end up as a concatenation of the values, space separated.
For example, this:
```C++
Httag p( std::cout, HT_P, "text" );
td.addAttrib( AT_CLASS, "abc" );
td.addAttrib( AT_CLASS, "cde" );
p.printTag();
```
will printout the correct code: `<p class="abc cde">text</p>`

For global attributes, it works the same.
Please note that the global attribute will always be added at the end of the string.


### A.4 - Run-time options

Different settings can be set at run-time, using static functions.

<a name="linefeed"></a>
#### A.4.1 - Line feeds

In order to be human readable, it may be a good idea to have here and then some line feeds in the output html code.
On the other side, for large files it may be wanted to have "compact" html code, by removing all the unnecessary linefeeds.

These two behaviors can be achieved, three modes are available:
- a line feed after every closing of a tag;
- no line feed at all;
- a "default" mode, where certain tags get a line feed after closure, others do not.

This is achieved with a static method:<br>
`setLineFeedMode( En_LineFeedMode mode )`<br>
The possible values are `LF_None`, `LF_Always`, `LF_Default`.

For example:
```C++
Httag::setLineFeedMode( LF_Always );
```

#### A.4.2 - Behavior on tag closing

Two different behaviors can be selected, globally.
Either the tag content is cleared automatically, either it is retained.
This can be selected with:
```C++
Httag::setClosingTagClearsContent( bool );
```
The default value is `false`.

### A.5 - Error handling

In case of non fatal problem, this library will simply issue a warning with a clear message on `std::cerr`.
This can be disabled by passing the option `HTTAG_SILENT_WARNINGS` before including the file.

In case of fatal error, this library will issue a message on `std::cerr` with line number and the error message, and throw an error of type `std::runtime_error`.

Similarly, error messages can be removed by defining the symbol `HTTAG_SILENT_ERRORS`.
You can mute both errors and warnings by defining `HTTAG_SILENT_MODE`. This will guarantee that no output will be streamed in `std::cerr`.

#### HTML 5 tag/attribute enforcing

The HTML 5 standard specifies two kinds of attributes:
- "global" attributes: these can be assigned to any tag
- regular attributes: each of these can be assigned to a limited number of tags

This library can ensure that this is enforced by checking at each time you add an attribute to a tag.
This checking can be disabled by defining the symbol `HTTAG_NO_CHECK` before the "include" line.


## <a name="macro"></a>B - Macro-based user interface

Several macros are available to mimic the behavior of the classical API.
The advantage is that in case of illegal HTML code, instead of throwing an error, using these macros will:

 1. print out a clear message on stderr and give the location of your faulty line,
 2. continue to process you code.

To avoid name collisions, they are prefixed with `HTTAG_`.

Macro                      | Equivalent code
-------------------------- | --------------
`HTTAG_OPENTAG( t );`      | `t.openTag();`
`HTTAG_CLOSETAG( t );`     | `t.closeTag();`
`HTTAG_ADD_ATTRIB( t, AT_CLASS, "abc")` | `t.addAttrib( AT_CLASS, "abc" )`


## <a name="errors"></a>C - List of errors

This code enforce several rules to avoid producing invalid HTML code.
This is done by throwing an error in case of incorrect usage by the client code.


Error  | Outcome  |  Example
-------|---------------------------------------|----------------
Open a tag that is not a file-type tag | fatal | `Httag t( HT_P );`<br>`t.openTag();`
Open a tag that is already open  | fatal | `Httag t( f, HT_P );`<br>`t.openTag();`<br>`t.openTag();`
Add an attribute to a tag where that attribute is not allowed | fatal | `Httag t( HT_TD );`<br>`t.addAttrib( AT_DATA, "abc" );`<br>or<br>`Httag t( HT_TD, AT_DATA, "abc" );`
Add content to a void tag | fatal | `Httag t( HT_BR );`<br>`t.addContent( "abc" );`
Add an attribute to a file-type tag already opened | fatal | `Httag t( f, HT_H2 );`<br>`t.openTag();`<br>`t.addAttrib( AT_CLASS, "abc" )`
Open a tag inside a context where it is not allowed | fatal | `Httag p( f, HT_P ); p.openTag();`<br>`Httag li( f, HT_LI ); li.openTag() );`


<hr>

### Developer information

Check out [Developer information](dev_info.md).


Copyright Sebastien Kramm - 2018-2020
