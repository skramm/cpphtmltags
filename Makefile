# makefile for cpphtmltags

.PHONY = clean demo test doc show diff

SRC_FILE=cpphtmltags.hpp
TEMPLATE_FILES := $(wildcard src/*.hh)
BUILD_SCRIPTS  := $(wildcard *.sh)
REF_FILES      := $(wildcard ref/*.ref)
MD_FILES       := $(wildcard *.md)
MD_FILES       += $(wildcard docs/*.md)
SRC_DEMO_FILES := $(wildcard demo/*.cpp)
EXE_DEMO_FILES := $(patsubst demo/%.cpp, build/%, $(SRC_DEMO_FILES))
TEST_FILE      :=test_A.cpp

# -g: for debut, if needed
# -s: strip unnecessary strings
#CFLAGS := -std=c++11 -g
#CFLAGS := -std=c++11 -s -DHTTAG_NO_REFERENCE_TABLES
CFLAGS := -std=c++11 -s

all: test demo doc

# -s option: also prints successful tests
test: build/testapp
	@echo "-Start test"
	@build/testapp
#	@build/testapp -s

# for unit testing
build/testapp: $(SRC_FILE) $(TEST_FILE) Makefile
	@echo "-Start compiling $(TEST_FILE)"
	$(CXX) $(CFLAGS) $(TEST_FILE) -o build/testapp

$(SRC_FILE): $(TEMPLATE_FILES) $(REF_FILES) $(BUILD_SCRIPTS)
	./build.sh

# builds the sample programs
demo: $(EXE_DEMO_FILES) Makefile
	@echo done

build/%:demo/%.cpp $(SRC_FILE) Makefile
	$(CXX) -o $@ $(CFLAGS) $<


clean:
	-rm build/*

# builds doxygen documentation
doc: build/html/index.html


cleandoc:
	-rm build/html/*

build/html/index.html:$(SRC_FILE) build/supported $(MD_FILES)
	cp misc/supported.css build/
	build/supported -html >build/supported.html
	doxygen misc/Doxyfile 1>build/doxygen.stdout 2>build/doxygen.stderr
	xdg-open build/html/index.html

show:
	@echo "REF_FILES=$(REF_FILES)"
	@echo "SRC_DEMO_FILES= $(SRC_DEMO_FILES)"
	@echo "EXE_DEMO_FILES=$(EXE_DEMO_FILES)"
	@echo "BUILD_SCRIPTS=$(BUILD_SCRIPTS)"
	@echo "TEMPLATE_FILES=$(TEMPLATE_FILES)"

# for dev purpose, shows differences in browser
diff:
	git diff ':!$(SRC_FILE)' | colordiff | aha>/tmp/diff.html
	xdg-open /tmp/diff.html

