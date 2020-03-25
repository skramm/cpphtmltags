# makefile for cpphtmltags

.PHONY = clean demo test doc show

SRC_FILE=cpphtmltags.hpp
TEMPLATE_FILES := $(wildcard src/*.hh)
REF_FILES      := $(wildcard ref/*.ref)
SRC_DEMO_FILES := $(wildcard demo/*.cpp)
EXE_DEMO_FILES := $(patsubst demo/%.cpp, build/%, $(SRC_DEMO_FILES))
TEST_FILE      :=test_A.cpp

# -g: for debut, if needed
# -s: strip unnecessary strings
#CFLAGS := -std=c++11 -g
CFLAGS := -std=c++11 -s


test: build/a.out
	@echo "-Start test"
	@build/a.out
#	@./a.out -s

# for unit testing
build/a.out: $(SRC_FILE) $(TEST_FILE) Makefile
	@echo "-Start compiling $(TEST_FILE)"
	$(CXX) $(CFLAGS) $(TEST_FILE) -o build/a.out

$(SRC_FILE): $(TEMPLATE_FILES) $(REF_FILES) build.sh
	./build.sh

# builds the sample programs
demo: $(EXE_DEMO_FILES) Makefile
	@echo done

build/%:demo/%.cpp $(SRC_FILE)
	$(CXX) -o $@ $(CFLAGS) $<


clean:
	-rm build/*


# builds doxygen documentation
doc:$(SRC_FILE)
	doxygen misc/Doxyfile 1>build/doxygen.stdout 2>build/doxygen.stderr
	xdg-open build/html/index.html

show:
	@echo $(REF_FILES)
	@echo $(SRC_DEMO_FILES)
	@echo $(EXE_DEMO_FILES)
	@echo $(TEST_FILE)

# for dev purpose, shows differences in browser
diff:
	git diff ':!$(SRC_FILE)' | colordiff | aha>/tmp/diff.html
	xdg-open /tmp/diff.html
	
