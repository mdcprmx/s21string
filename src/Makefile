COMPILE_W_USUAL_FLAGS = gcc -Wall -Wextra -Werror -Wpedantic -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS_FLAGS = -lcheck -lm -lsubunit

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

SRC_TESTS = ./tests/*.c

all: s21_string.a test gcov_report

%.o: %.c
	$(COMPILE_W_USUAL_FLAGS) -c $< -o $@

s21_string.a: $(OBJ_FILES)
	ar rcs $@ $^
	rm -f $(OBJ_FILES)


#####

BUILD_UNIT_TESTS = $(COMPILE_W_USUAL_FLAGS) $(SRC_TESTS) s21_string.a -o $(TEST_OUT) $(LIBS_FLAGS)
TEST_OUT = ./tests/test.out

test: s21_string.a
	$(BUILD_UNIT_TESTS)
	$(TEST_OUT)

gcov_report: clean $(OBJ_FILES)
	$(COMPILE_W_USUAL_FLAGS) $(GCOV_FLAGS) $(SRC_TESTS) $(SRC_FILES) $(LIBS_FLAGS) -o ./tests/report.out 
	./tests/report.out
	lcov --capture --directory ./tests/ -o ./tests/report.info
	genhtml ./tests/report.info --frames --output-directory report_html
	open ./report_html/src/index.html

clangn:
	clang-format -style=Google -n *.c *.h

clangi:
	clang-format -style=Google -i *.c *.h

clean:
	rm -rf report_html
	rm -rf ./tests/*.info ./tests/*.out
	rm -f *.o *.a tests/unit_tests *.gcno *.gcda tests/*.gcno tests/*.gcda *.out *.gcov
	rm -f $(OBJ_FILES)
