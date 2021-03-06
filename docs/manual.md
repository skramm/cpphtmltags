# Manual for cpphtmltags library

Homepage: https://github.com/skramm/cpphtmltags/


## Table of contents
1. [Introduction](#intr)
2. [Instanciation](#constructor)
3. [Adding content](#content)
4. [Attributes](#attribs)
5. [Runtime options](#options)
6. [Error handling](#errors)
7. [Macro API](#macro)

<a name="intro"></a>
## 1 - Introduction

This library is build around a single class: `Httag`, along with some helper free functions.
This class models an HTML tag (aka element), that can be streamed in a file or output stream.
The whole API is in the namespace `httag`.

Two types of tags can be created, but they are both handled trough the same class.
They only differ in the way they are created, and how they generate some output.
- The first type is independent of any output device, it is up to the user to stream them in some output stream.
- With the second type, you assign an output stream at creation time. It is stored internally, and you can then explicitely control the output process.

The library and the contained class can be used in two ways, either through classical object paradigm, either through a set of macros.
These basically wrap the code, it ends up as the same thing, with one notable exception:
All the potential errors you might do while writing your code (say, trying to write something like `<p><p>abc</p></p>`, which is illegal) will throw an error.
- With the classical API, your program will stop (unless you try to catch them, but that would be tedious).
- With the macro-based API, your program will continue, and you will get in stderr a nice information log, with the location of the error in your code.
However, due to the macro mechanism, you have much less flexibility, only a subset of the API is available.
For more details, [see here](#macro).


<a name="constructor"></a>
## 2 - Instanciation

Two types of constructors are available, corresponding to the two types of objects.

### 2.1 - Type 1 constructor
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

### 2.2 - Type 2 constructor ("file-type" object)

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


Once it is opened, you can explicitly close it with `closeTag()`.
But this also called automatically by the destructor, which is called when the variable goes out of scope.

### 2.3 - Which one should I use ?

Theorically, both could achieve the same html structure.

The first type of tags (that has no associated stream/file) can be used in a hierarchical way:
you can stream into it a `Httag` variable, either an explicit one, or an anonymous one, through a constructor call.
For example, a minimal page holding 2 divs, one empty and the second one holding a paragraph could be written this way:
```C++
std::cout << Httag( HT_DOCTYPE );
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

<a name="content"></a>
## 3 - Adding content to a tag

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

Even this is possible:
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

<a name="attribs"></a>
## 4 - Attributes

### 4.1 - Adding attributes
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

### 4.2 - Boolean attributes

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

### 4.3 - Attributes uniqueness enforcement

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

<a name="options"></a>
## 5 - Run-time options

Different settings can be set at run-time, using the static function `setOption( a, b )`.
The first argument is an enum value, used to define what option must be edited, the second argument is type dependent.
These are all in the sub-namespace `rto`.

<a name="linefeed"></a>
### 5.1 - Line feeds

In order to be human readable, it may be a good idea to have here and then some line feeds in the output Html code.
On the other side, for large files it may be wanted to have "compact" html code, by removing all the unnecessary linefeeds.

These two behaviors can be achieved, three modes are available:
- a line feed after every closing of a tag;
- no line feed at all;
- a "default" mode, where certain tags get a line feed after closure, others do not.

This is achieved with the above static method, using as first argument `rto::LFMode`<br>
For the second argument, the possible values are `rto::LF_None`, `LF_Always`, `LF_Default`.

For example:
```C++
Httag::setOption( rto::LFMode, rto::LF_Always );
```

### 5.2 - Behavior on tag closing

The behavior of a tag object upon closure can be defined globally.

Consider a tag holding a content and some attributes:
```C++
Httag p( HT_P, "text", AT_CLASS, "c1" );
```
What happens when we do this?:
```C++
f << p << p;
```
The default behavior is leave tag unchanged, so we get:
`<p class="c1">text</p><p class="c1">text</p>`

However, in some situation, it can be desired that the tag gets "cleared" once it is closed.
This can be defined globally with the above function:
- either only for the content `Httag::setOption( rto::ClearContentOnClose, true/false );`,
- only for the attributes `Httag::setOption( rto::ClearAttribsOnClose, true/false );`,
- or for both `Httag::setOption( rto::ClearOnClose, true/false );`.

### 5.3 - Behavior on errors

TODO

first argument: httag::rto::IllegalOp

possible values: `rto::EM_Throw`, `rto::EM_NoThrow`

<a name="errors"></a>
## 6 - Errors and warnings handling

Every possible misusage that could generate invalid Html5 code will generate either a simple warning, an error, or a fatal error.

These 3 situations will stream some useful information on `stderr`, unless the build option `HTTAG_SILENT_MODE` is defined.

A fatal error will always throw an exception (of type `std::runtime_error`).
For the two others, it depends on the runtime option `httag::rto::IllegalOp`


### 6.1 - HTML 5 tag/attribute enforcing

The HTML 5 standard specifies two kinds of attributes:
- "global" attributes: these can be assigned to any tag
- regular attributes: each of these can be assigned to a limited number of tags

This library can ensure that this is enforced by checking at each time you add an attribute to a tag.
This checking can be disabled by defining the symbol `HTTAG_NO_CHECK` before the "include" line.


<a name="macro"></a>
## 7 - Macro-based user interface

Several macros are available to mimic the behavior of the classical API.
The advantage is that in case of illegal HTML code, instead of throwing an error, using these macros will:

 1. print out a clear message on stderr and give the location of your faulty line,
 2. continue to process you code.

To avoid name collisions, they are prefixed with `HTTAG_`.

Macro                      | Equivalent code
-------------------------- | --------------
`HTTAG_OPEN( t );`      | `t.openTag();`
`HTTAG_CLOSE( t );`     | `t.closeTag();`
`HTTAG_ADDATTRIB( t, AT_CLASS, "abc")` | `t.addAttrib( AT_CLASS, "abc" )`
`HTTAG_ADD( t, "text")` | `t.addContent( "text" )`
`HTTAG_SET( t, "text")` | `t.setContent( "text" )`

These macros basically wrap the corresponding code into a `try/catch` structure.


<a name="errors"></a>
## - List of errors

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
