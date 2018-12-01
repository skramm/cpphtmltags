#!/bin/bash

# generation of C++ code
# builds map of categories of tags


file_input=ref/tag_content_2.ref
file_out=tmp/tag_content.src

echo "/// Holds for each tag its allowed content"> $file_out
echo -e "struct TagContent\n{">> $file_out

echo -e "\tstd::map<En_Httag,AllowedContent> _map_AllowedContent;">> $file_out

#echo -e "\tTagAllowedContent<En_TagCat> _map_AllowedCats;">> $file_out
#echo -e "\tTagAllowedContent<En_Httag> _map_AllowedTags;">> $file_out
#echo -e "\tTagAllowedContent<En_Httag> _map_ForbiddenTags;">> $file_out

echo -e "\tAllowedContent& get( En_Httag tag )\n\t{">> $file_out
echo -e "\t\tassert( _map_AllowedContent.count(tag) );">> $file_out
echo -e "\t\treturn _map_AllowedContent[tag];\n\t}">> $file_out

echo -e "\n\tvoid print( std::ostream& ) const;">> $file_out

echo -e "\n\tTagContent()\n\t{">> $file_out


while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then
		if [ ${#a} -ne 0 ]; then
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
			echo -e "\t\t{\n\t\t\tAllowedContent ac;">> $file_out
			if [ "${b:0:2}" = "E_" ]; then
				if [ "$b" = "E_VOID" ]; then
					echo -e "\t\t\tac._isVoid=true;">> $file_out
				fi
				if [ "$b" = "E_TEXT" ]; then
					echo -e "\t\t\tac._isText=true;">> $file_out
				fi
			else
				IFS=',' read -ra TAG <<< "$b"
				for t in "${TAG[@]}";
				do
					t2=$(echo $t | tr '[:lower:]' '[:upper:]')
					if [ "${t2:0:2}" = "C_" ]; then
						echo -e "\t\t\tac.addCat($t2);">> $file_out
					else
						if [ "${t2:0:1}" = "!" ]; then
							t3=${t2:1}
							if [ "${t3:0:2}" = "C_" ]; then
								echo -e "\t\t\tac.addForbiddenCat($t3);">> $file_out
							else
								echo -e "\t\t\tac.addForbiddenTag(HT_$t3);">> $file_out
							fi
						else
							echo -e "\t\t\tac.addTag(HT_$t2);">> $file_out
						fi
					fi
				done
			fi
			echo -e "\t\t\t_map_AllowedContent[HT_$at] = ac;">> $file_out
			echo -e "\t\t}">> $file_out
		fi
	fi
done < $file_input


echo -e "\t}\n};" >>$file_out





