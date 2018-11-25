# makefile for cpphtmltags


# not used at present but could be for multiple test files
#SRC_TEST_FILES = $(wildcard tests/test_*.cpp)
#EXE_TEST_FILES := $(patsubst tests/%.cpp, bin/%, $(SRC_TEST_FILES))
TEST_FILE=tests/test_A.cpp

CFLAGS := -std=c++11

#test: $(EXE_TEST_FILES)
test: a.out
	@echo "-Start test"
	@./a.out
#	@./a.out -s

# for tests
a.out: cpphtmltags.hpp $(TEST_FILE) Makefile
	@echo "-start compiling $(TEST_FILE)"
	@$(CXX) $(CFLAGS) $(TEST_FILE)


# builds the sample programs


# builds doxygen documentation
doc:
	doxygen Doxyfile
	xdg-open html/index.html
