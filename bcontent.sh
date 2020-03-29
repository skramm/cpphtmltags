#!/bin/bash

# file: bcontent.sh
# author: S. Kramm

# generation of C++ code
# builds map of content allowed into a tag
# build class httag::priv::AllowedContentMap

#**************************************************
function print_header()
{
	echo "// -------- GENERATED CODE ! --------"> $1
	echo -e "// timestamp: $(date +%Y%m%d-%H%M)\n">> $1
}

#**************************************************


echo "Running bcontent.sh"

file_input=ref/tag_content.ref
file_out=build/tmp/tag_content.src
file_out2=build/tmp/is_void.src
file_out3=build/tmp/is_text.src

# START
set +x
mkdir -p build/tmp

print_header $file_out
echo "/// Holds for each tag its allowed content. Build-time generated type.">> $file_out
echo "/// See related type AllowedContent.">> $file_out
echo -e "struct AllowedContentMap\n{">> $file_out

echo -e "\tstd::map<En_Httag,AllowedContent> _map_AllowedContent;">> $file_out

echo -e "\tAllowedContent& get( En_Httag tag )\n\t{">> $file_out
echo -e "\t\tassert( _map_AllowedContent.count(tag) );">> $file_out
echo -e "\t\treturn _map_AllowedContent[tag];\n\t}">> $file_out

echo -e "\tconst AllowedContent& get( En_Httag tag ) const\n\t{">> $file_out
echo -e "\t\tassert( _map_AllowedContent.count(tag) );">> $file_out
echo -e "\t\treturn _map_AllowedContent.at(tag);\n\t}">> $file_out

echo -e "\n\tvoid print( std::ostream& ) const;">> $file_out
echo -e "\tAllowedContentMap()\n\t{">> $file_out
echo -e "\n/// Constructor">> $file_out

print_header $file_out2
echo -e "/// Returns true if the tag is a void-element">> $file_out2
echo -e "inline\nbool\nisVoidElement( En_Httag tag )\n{">> $file_out2
echo -e "\tswitch( tag )\n\t{">> $file_out2

print_header $file_out3
echo -e "/// Returns true if the tag is a text-only element">> $file_out3
echo -e "inline\nbool\nisTextOnly( En_Httag tag )\n{">> $file_out3
echo -e "\tswitch( tag )\n\t{">> $file_out3

while IFS=$':' read a b
do
	if [ "${a:0:1}" != "#" ]; then                         # if not a comment
		if [ ${#a} -ne 0 ]; then                           # if not empty line
			at=$(echo $a | tr '[:lower:]' '[:upper:]')
			if [ "${b:0:2}" = "E_" ]; then                 # if either "void" or "text content" type
				if [ "$b" = "E_VOID" ]; then
					echo -e "\t\t{\n\t\t\tAllowedContent ac(AllowedContent::TT_VOID);">> $file_out
					echo -e "\t\tcase HT_$at:">> $file_out2
				fi
				if [ "$b" = "E_TEXT" ]; then
					echo -e "\t\t{\n\t\t\tAllowedContent ac(AllowedContent::TT_TEXT);">> $file_out
					echo -e "\t\tcase HT_$at:">> $file_out3
				fi
			else                                          # if not, just default-instanciate
				echo -e "\t\t{\n\t\t\tAllowedContent ac;">> $file_out
				IFS=',' read -ra TAG <<< "$b"
#				echo "a=$a b=$b, processing $TAG"
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

echo -e "\t\t\treturn true;\n\t\tdefault:\n\t\t\tbreak;\n\t}\n\treturn false;\n}">> $file_out2
echo -e "\t\t\treturn true;\n\t\tdefault:\n\t\t\tbreak;\n\t}\n\treturn false;\n}">> $file_out3




