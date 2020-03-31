#!/bin/bash

# file: build.sh
# author: S. Kramm

## This file is part of cpphtmltags
## it generates some C++ code that will get embedded in the library
## it takes as input some reference html data, as raw text files in "ref" folder

OUT_FILE=cpphtmltags.hpp

echo "Running build.sh"

mkdir -p build/tmp
src/bcat.sh
src/bcontent.sh
src/btags.sh



echo "// -------- GENERATED CODE ! --------"> $OUT_FILE
echo -e "// timestamp: $(date +%Y%m%d-%H%M)\n">> $OUT_FILE


cat \
	src/cpphtmltags_1.hh \
	build/tmp/tags_enum.src \
	build/tmp/attribs_enum.src \
	build/tmp/tags_switch.src \
	build/tmp/attribs_switch.src \
	build/tmp/cat_enum.src \
	build/tmp/cat_switch.src \
	src/cpphtmltags_2.hh \
	build/tmp/attrib_tags.src \
	build/tmp/cat_element.src \
	build/tmp/is_void.src \
	build/tmp/is_text.src \
	build/tmp/tag_content.src \
	build/tmp/global_attribs.src \
	build/tmp/bool_attribs.src \
	src/cpphtmltags_3.hh \
	>> $OUT_FILE

