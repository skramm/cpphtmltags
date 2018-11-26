# Developper information

## Generation of library file

Starting from release 1.0, the actual library file (`cpphtmltags.hpp`) is actually generated from several files.
The main c++ code is in `cpphtmltags_1.hh` and `cpphtmltags_2.hh`

All the html reference material is located in the files located in the `ref` subfolder:
- `tags.ref` holds the list of tags
- `global_attribs.ref` holds the list of global attributes (the one that can be used with any html element).
- `valid_attribs.ref` holds the list of authorized tags for a given attribute, in the form `attrib:tag1,tag2,tag3`.
It is used along with `global_attribs.ref` to generate a file holding all the attributes, that is used to generate the code.
- `void_elements.ref` holds the list of void-elements (tags that don't get closed)

These files serve a source to build the actual code.
This done by the shell script `build.sh`.

Four steps:
- build the reference file holding all the attributes
- build tags and attributes enum and `getString()` functions from plain text files
- build c++ code to initialize a map holding for each attribute the tags where they are allowed.
- concatenate first part of header, this generated code, and final part of header to generate the main header file.

The advantage of such an approach is that in case new tags or attributes are integrated into the standard, only these files need to be edited.

## Static variables

All the reference stuff (tags/attribs strings, map of allowed attributes, ...) are stored in static variables.
In order to make this work in a header file, the classical "lazy static initialization" trick is used:
The variable is stored in a static function, that just returns the variable.
The variable is build upon first call of the function.
