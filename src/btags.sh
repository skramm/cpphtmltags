#!/bin/bash

# file: build.sh
# author: S. Kramm

# This file is part of cpphtmltags
# it generates some C++ code that will get embedded in the library
# it takes as input some reference html data, as raw text files in "ref" folder


echo "Running $(basename $0)"

#**************************************************
function print_header()
{
	echo "// -------- GENERATED CODE ! --------"> $1
	echo -e "// timestamp: $(date +%Y%m%d-%H%M)\n">> $1
}

#**************************************************
function generate()
{
	print_header $file_s
	print_header $file_e

	echo -e "std::string\ngetString( $name a )">> $file_s
	echo -e "enum $name\n{">> $file_e

	echo -e "{\n\tconst char* n=0;">> $file_s
	echo -e "\tswitch( a )">> $file_s
	echo -e "\t{">> $file_s

	while read str
	do
		if [ "${str:0:1}" != "#" ]; then
			if [ ${#str} -ne 0 ]; then
				v=$(echo $str | tr '[:lower:]' '[:upper:]')
				echo -e "\t\tcase ${pre}_$v: n = \"$str\"; break;">> $file_s
				echo -e "\t${pre}_$v," >> $file_e
			fi
		fi
	done < $file_input

	echo -e "\n\t\tdefault: assert(0);">> $file_s
	echo -e "\t}">> $file_s
	echo -e "\treturn n;\n}\n">> $file_s

	echo -e "\n\t${pre}_DUMMY">> $file_e
	echo -e "};\n">> $file_e
}

#**************************************************
function extract()
{
	rm $file_output
	while IFS=$':' read a b
	do
		if [ "${a:0:1}" != "#" ]; then
			if [ ${#a} -ne 0 ]; then
				echo "$a" >> $file_output
			fi
		fi
	done < $file_input
}

#**************************************************


# STEP 0: generate list of attributes from authorized tags/attributes file

file_input=ref/valid_attribs.ref
file_output=build/tmp/attribs_1.ref
extract

cat build/tmp/attribs_1.ref ref/global_attribs.ref> build/tmp/attribs_2.ref
sort <build/tmp/attribs_2.ref >build/tmp/attribs.ref

file_input=ref/tag_content.ref
file_output=build/tmp/tags.ref
extract


#*************************************************
# used vars: file_output, file_input, something, funcname

function generateIsSomething()
{
	echo "/// Returns true if \c attr if $something">$file_output
	echo "inline bool">>$file_output
	echo "$funcname( En_Attrib attr )">>$file_output
	echo -e "{">>$file_output
	echo -e "\tassert( attr != AT_DUMMY );">>$file_output
	echo -e "\tswitch( attr )">>$file_output
	echo -e "\t{">>$file_output

	while read elem
	do
		if [ "${elem:0:1}" != "#" ]; then
			if [ ${#elem} -ne 0 ]; then
				e=$(echo $elem | tr '[:lower:]' '[:upper:]')
				echo -e "\t\tcase AT_$e:">> $file_output
			fi
		fi
	done < $file_input

	echo -e "\t\t\treturn true;">>$file_output
	echo -e "\t\tdefault:">>$file_output
	echo -e "\t\t\treturn false;">>$file_output
	echo -e "\t}">>$file_output
	echo -e "\treturn false;">>$file_output
	echo -e "}">>$file_output
}

# STEP 0.1: generate isGlobalAttr() and is isBoolAttr()

file_input=ref/global_attribs.ref
file_output=build/tmp/global_attribs.src
something=global
funcname=isGlobalAttr
generateIsSomething

file_input=ref/bool_attribs.ref
file_output=build/tmp/bool_attribs.src
something=boolean
funcname=isBoolAttr
generateIsSomething


# STEP 1: generate enum and getString() functions, for tags and attributes
# 1.1 - for tags
file_input=build/tmp/tags.ref
file_e=build/tmp/tags_enum.src
file_s=build/tmp/tags_switch.src
pre=HT
name=En_Httag
generate

# 1.2 - for attributes
file_input=build/tmp/attribs.ref
file_e=build/tmp/attribs_enum.src
file_s=build/tmp/attribs_switch.src
pre=AT
name=En_Attrib
generate



# STEP 3: generate map of allowed tags for each attribute

file_input=ref/valid_attribs.ref
file_out=build/tmp/attrib_tags.src

print_header $file_out

echo "/// Private class, holds map of allowed tags (value) for a given attribute (key)">> $file_out
echo -e "struct MapAttribs\n{">> $file_out
echo -e "\tMapAttribs_t _map;">> $file_out
echo -e "\tconst MapAttribs_t& get() {">> $file_out
echo -e "\t\treturn _map;\n\t}">> $file_out
echo -e "\tMapAttribs()\n\t{" >>$file_out

while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
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
		fi
	fi
done < $file_input

echo -e "\t}\n};" >>$file_out

