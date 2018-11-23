# makefile for cpphtmltags


SRC_TEST_FILES = $(wildcard tests/test_*.cpp)
EXE_TEST_FILES := $(patsubst tests/%.cpp, bin/%, $(SRC_TEST_FILES))


CFLAGS := -std=c++11

test: $(EXE_TEST_FILES)
	@echo "done target $<"

bin/%:	tests/%.cpp
	@$(CXX) $(CFLAGS) $<
	@echo "Start test, file $<"
	@./a.out -s
	@echo "Done test"

show:
	@echo "EXE_TEST_FILES=$(EXE_TEST_FILES)"

# builds the sample programs

doc:
	doxygen Doxyfile
	xdg-open html/index.html
