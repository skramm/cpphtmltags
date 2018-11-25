#!/bin/bash

# This file is part of cpphtmltags

function generate()
{
echo -e "const char* getString( $name a )"> $file_s
echo -e "enum $name\n{"> $file_e

echo -e "{\n\tconst char* n=0;">> $file_s
echo -e "\tswitch( a )">> $file_s
echo -e "\t{">> $file_s
while read str
do
	v=$(echo $str | tr '[:lower:]' '[:upper:]')
	echo -e "\t\tcase ${pre}_$v: n = \"$str\"; break;">> $file_s
	echo -e "\t${pre}_$v," >> $file_e
done < $file_input

echo -e "\n\t\tdefault: assert(0);">> $file_s
echo -e "\t}">> $file_s
echo -e "\treturn n;\n}">> $file_s

echo -e "\n\t${pre}_DUMMY">> $file_e
echo -e "};">> $file_e
}

file_input=src_tags
file_e=tmp/tags_enum.src
file_s=tmp/tags_switch.src
pre=HT
name=En_Htag
generate

file_input=src_attribs
file_e=tmp/attribs_enum.src
file_s=tmp/attribs_switch.src
pre=AT
name=En_Attrib
generate

cat
