#!/bin/bash

# This file is part of cpphtmltags
# it generates C++ code that will ge embedded in the library
# it takes as input some reference html data, as raw text files in "ref" folder

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
echo -e "\treturn n;\n}\n">> $file_s

echo -e "\n\t${pre}_DUMMY">> $file_e
echo -e "};\n">> $file_e
}

# STEP 0: generate list of attributes from authorized tags/attributes file

file_input=ref/valid_attribs.ref
file_output=tmp/attribs_1.ref

rm $file_output
while IFS=$':' read a b
do
	echo "$a" >> $file_output
done < $file_input
cat tmp/attribs_1.ref ref/global_attribs.ref> tmp/attribs_2.ref
sort <tmp/attribs_2.ref >tmp/attribs.ref


# STEP 1: generate enum and getString() functions, for tags and attributes
file_input=ref/tags.ref
file_e=tmp/tags_enum.src
file_s=tmp/tags_switch.src
pre=HT
name=En_Httag
generate

file_input=tmp/attribs.ref
file_e=tmp/attribs_enum.src
file_s=tmp/attribs_switch.src
pre=AT
name=En_Attrib
generate

# STEP 2: generate map of allowed tags for each attribute

file_input=ref/valid_attribs.ref
file_out=tmp/attrib_tags.src

echo "/// Conveniency typedef"> $file_out
echo "typedef std::map<En_Attrib"",std::vector<En_Httag>> MapAttribs_t;">> $file_out

echo "/// Private class, holds map pf allowed attributes">> $file_out
echo -e "struct MapAttribs\n{">> $file_out
echo -e "\tMapAttribs_t _map;">> $file_out
echo -e "\tconst MapAttribs_t& get() {">> $file_out
echo -e "\t\treturn _map;\n\t}">> $file_out
echo -e "\tMapAttribs()\n\t{" >>$file_out

while IFS=$':' read a b
do
	at=$(echo $a | tr '[:lower:]' '[:upper:]')
#	echo "a=$a at=$at b=$b"
	echo -en "\t\t_map[AT_$at]\t=\t { ">> $file_out
	IFS=',' read -ra TAG <<< "$b"
	n=0
	nb=${#TAG[@]}
	for t in "${TAG[@]}";
	do
		tag=$(echo $t | tr '[:lower:]' '[:upper:]')
		echo -n "HT_$tag" >> $file_out
		((n++))
		if [ $n != $nb ]
		then
			echo -n ", " >> $file_out
		fi
	done
	echo -e " };">> $file_out
done < $file_input

echo -e "\t}\n};" >>$file_out

# STEP 3 : generate header file
cat \
	cpphtmltags_1.hh \
	tmp/tags_enum.src \
	tmp/attribs_enum.src \
	tmp/tags_switch.src \
	tmp/attribs_switch.src \
	tmp/attrib_tags.src \
	cpphtmltags_2.hh \
	> cpphtmltags.hpp \
