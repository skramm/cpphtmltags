#!/bin/bash

file_input=src_valid_attribs
file_out=tmp/attrib_tags.src

rm $file_out


while IFS=$':' read a b
do
	at=$(echo $a | tr '[:lower:]' '[:upper:]')
	echo "a=$a at=$at b=$b"
	echo -en "\t_map[AT_$at] \t= \t{ ">> $file_out
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



