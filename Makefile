# makefile for cpphtmltags


SRC_FILE=cpphtmltags.hpp

REF_FILES      := $(wildcard ref/*.ref)
SRC_DEMO_FILES := $(wildcard demo/*.cpp)
EXE_DEMO_FILES := $(patsubst demo/%.cpp, %, $(SRC_DEMO_FILES))
TEST_FILE      :=tests/test_A.cpp

CFLAGS := -std=c++11

test: a.out
	@echo "-Start test"
	@./a.out
#	@./a.out -s

# for unit testing
a.out: $(SRC_FILE) $(TEST_FILE) Makefile
	@echo "-Start compiling $(TEST_FILE)"
	@$(CXX) $(CFLAGS) $(TEST_FILE)

$(SRC_FILE): cpphtmltags_1.hh cpphtmltags_2.hh $(REF_FILES)
	./build.sh

demo: $(EXE_DEMO_FILES) Makefile
	@echo done

%:demo/%.cpp $(SRC_FILE)
	$(CXX) -o $@ $(CFLAGS) $<

# builds the sample programs


# builds doxygen documentation
doc:
	doxygen Doxyfile
	xdg-open html/index.html

show:
	@echo $(REF_FILES)
	@echo $(SRC_DEMO_FILES)
	@echo $(EXE_DEMO_FILES)
	@echo $(TEST_FILE)
