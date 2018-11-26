# makefile for cpphtmltags


# not used at present but could be for multiple test files
SRC_DEMO_FILES = $(wildcard demo/*.cpp)
EXE_DEMO_FILES := $(patsubst demo/%.cpp, %, $(SRC_DEMO_FILES))
TEST_FILE=tests/test_A.cpp

CFLAGS := -std=c++11

test: a.out
	@echo "-Start test"
	@./a.out
#	@./a.out -s

# for unit testing
a.out: cpphtmltags.hpp $(TEST_FILE) Makefile
	@echo "-start compiling $(TEST_FILE)"
	@$(CXX) $(CFLAGS) $(TEST_FILE)

demo: $(EXE_DEMO_FILES) Makefile
	@echo done

%:demo/%.cpp
	$(CXX) -o $@ $(CFLAGS) $<

# builds the sample programs


# builds doxygen documentation
doc:
	doxygen Doxyfile
	xdg-open html/index.html
