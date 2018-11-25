# Developper information

Starting from release XX, the actual library file (cpphtmltags.hpp) is actually generated from several files.
The main c++ code is in `cpphtmltags_1.src` and `cpphtmltags_2.src`

All the html material is located in the files located in the `ref` subfolder:
- `attribs.ref` holds the list of attributes
- `tags.ref` holds the list of tags
- `valid_attribs.ref` holds the list of authorized tags for a given attribute, in the form `attrib:tag1,tag2,tag3`
- `void_elements.ref` holds the list of void-elements (tags that don't get closed)

These files serve a source to build the actual code.
This done by the shell script

Three steps:
- build tags and attributes enum and getString() functions from plain test files `attribs.ref` and `tags.ref`
- build c++ code to initialize a map holding for each attribute the tags where they are allowed.
- concatenate first part of header, this generated code, and final part of header to generate the file.

