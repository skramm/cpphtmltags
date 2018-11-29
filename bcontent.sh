#!/bin/bash

# generation of C++ code
# builds map of categories of tags


file_input=ref/tag_content.ref
file_out=tmp/tag_content.src

echo -e "\nstruct TagContent\n{"> $file_out
echo -e "\tTagContentC_t _map_content_C;">> $file_out
echo -e "\tTagContentT_t _map_content_T;">> $file_out
echo -e "\tTagContent_t get( En_Httag tag )\n\t{">> $file_out
echo -e "\t\treturn std::make_pair( _map_content_C[tag], _map_content_T[tag] );\n\t}">> $file_out
echo -e "\tTagContent()\n\t{">> $file_out


while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
#			echo -e "\t$at,">> $file_out
			echo -en "\t\t_map_content_C[HT_$at]\t=\t { ">> $file_out
			IFS=',' read -ra TAG <<< "$b"
			n=0
			nb=${#TAG[@]}
			for t in "${TAG[@]}";
			do
				t2=$(echo $t | tr '[:lower:]' '[:upper:]')
				if [ "${t2:0:1}" = "C" ]; then
					echo -n "$t2" >> $file_out
					((n++))
					if [ $n != $nb ]
					then
						echo -n ", " >> $file_out
					fi
				fi
			done
			echo -e " };">> $file_out
		fi
	fi
done < $file_input

echo "">> $file_out

while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
#			echo -e "\t$at,">> $file_out
			echo -en "\t\t_map_content_T[HT_$at]\t=\t { ">> $file_out
			IFS=',' read -ra TAG <<< "$b"
			n=0
			nb=${#TAG[@]}
			for t in "${TAG[@]}";
			do
				t2=$(echo $t | tr '[:lower:]' '[:upper:]')
				if [ "${t2:0:1}" != "C" ]; then
					echo -n "HT_$t2" >> $file_out
					((n++))
					if [ $n != $nb ]
					then
						echo -n ", " >> $file_out
					fi
				fi
			done
			echo -e " };">> $file_out
		fi
	fi
done < $file_input

echo -e "\t}\n};" >>$file_out





