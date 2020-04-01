#!/bin/bash

# file: bcat.sh
# author: S. Kramm

# This file is part of cpphtmltags
# generation of C++ code
# builds map of categories of tags
# this script takes as input one file and generates three files

echo "Running $(basename $0)"

# Input
file_input=ref/element_cat.ref

# output 1: holds the enum declaration httag::priv::En_TagCat
file_out1=build/tmp/cat_enum.src

# output 2: holds the declaration of class httag::priv::TagCategory that holds a map of categories of tags
file_out2=build/tmp/cat_element.src

# output 3: holds the getString( En_TagCat ) function
file_out3=build/tmp/cat_switch.src

# START
set +x

echo "//############################"> $file_out1
echo "namespace priv {">> $file_out1
echo "//############################">> $file_out1

echo "/// Enum holding tag categories">> $file_out1
echo "/** See https://html.spec.whatwg.org/multipage/dom.html#kinds-of-content */">> $file_out1
echo -e "enum En_TagCat\n{">> $file_out1


echo "/// Private class, holds map of categories of tags"> $file_out2
echo -e "struct TagCategory\n{">> $file_out2
echo -e "\tMapTagCat_t _map_cat;">> $file_out2
echo -e "\tconst MapTagCat_t& get() {">> $file_out2
echo -e "\t\treturn _map_cat;\n\t}">> $file_out2
echo -e "\tTagCategory()\n\t{" >>$file_out2

while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
			echo -e "\t$at,">> $file_out1
			echo -en "\t\t_map_cat[$at]\t=\t { ">> $file_out2
			IFS=',' read -ra TAG <<< "$b"
			n=0
			nb=${#TAG[@]}
			for t in "${TAG[@]}";
			do
				tag=$(echo $t | tr '[:lower:]' '[:upper:]')
				echo -n "HT_$tag" >> $file_out2
				((n++))
				if [ $n != $nb ]
				then
					echo -n ", " >> $file_out2
				fi
			done
			echo -e " };">> $file_out2
		fi
	fi
done < $file_input

echo -e "\n\tC_DUMMY\n};" >>$file_out1

echo -e "\t}\n};" >>$file_out2



echo -e "std::string\ngetString( En_TagCat a )"> $file_out3

echo -e "{\n\tconst char* n=0;">> $file_out3
echo -e "\tswitch( a )">> $file_out3
echo -e "\t{">> $file_out3

while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
			echo -e "\t\tcase $at:\tn=\"$a\";\tbreak;">> $file_out3
		fi
	fi
done < $file_input
echo -e "\t\tdefault: assert(0);">> $file_out3
echo -e "\t}\n\treturn n;\n}">> $file_out3
