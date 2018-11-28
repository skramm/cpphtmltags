#!/bin/bash

# generation of C++ code
# builds

set +x

file_input=ref/element_cat.ref
file_out1=tmp/element_cat_enum.src
file_out2=tmp/element_cat.src


# START

#echo "/// private namespace"> $file_out1
#echo -e "namespace priv {\n"> $file_out1
echo "/// Enum holding tag categories"> $file_out1
echo -e "enum En_TagCat\n{">> $file_out1

echo "/// Conveniency typedef"> $file_out2
echo -e "typedef std::map<En_TagCat,std::vector<En_Httag>> TagCat_t;\n">> $file_out2

echo "/// Private class, holds map of categories of tags">> $file_out2
echo -e "struct TagCat\n{">> $file_out2
echo -e "\tTagCat_t _map_cat;">> $file_out2
echo -e "\tconst TagCat_t& get() {">> $file_out2
echo -e "\t\treturn _map_cat;\n\t}">> $file_out2
echo -e "\tTagCat()\n\t{" >>$file_out2

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
#echo -e "} // namespace priv\n">> $file_out2



