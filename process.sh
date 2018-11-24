#!/bin/bash


function generate()
{
rm $file_e
rm $file_s

while read str
do
	v=$(echo $str | tr '[:lower:]' '[:upper:]')
	echo -e "\tcase ${pre}_$v: n = \"$str\"; break;" >> $file_s
	echo -e "\t${pre}_$v," >> $file_e
done < $file_input
}

file_input=src_tags
file_e=tmp/tags_enum.src
file_s=tmp/tags_switch.src
pre=HT
generate

file_input=src_attribs
file_e=tmp/attribs_enum.src
file_s=tmp/attribs_switch.src
pre=AT
generate


