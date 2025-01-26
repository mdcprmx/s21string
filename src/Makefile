USUAL_FLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

all: s21_string.a test gcov_report

%.o: %.c
	gcc $(USUAL_FLAGS) -c $< -o $@

s21_string.a: $(OBJ_FILES)
	ar rcs $@ $^
	rm -f $(OBJ_FILES)

test: s21_string.a
	gcc $(USUAL_FLAGS) tests/unit_tests.c s21_string.a -o tests/unit_tests -lcheck -lm -lsubunit
	./tests/unit_tests

gcov_report: clean $(OBJ_FILES)
	gcc $(GCOV_FLAGS) tests/unit_tests.c $(SRC_FILES) -o ./tests/report.out -lcheck -lm -lsubunit
	./tests/report.out
	lcov --capture --directory ./tests/ -o ./tests/report.info
	genhtml ./tests/report.info --frames --output-directory report_html
	open ./report_html/src/index.html

clangfromfile:
	clang-format -i *.c *.h tests/*.c
	clang-format -n *.c *.h tests/*.c

clangn:
	clang-format -style=Google -n *.c *.h

clangi:
	clang-format -style=Google -i *.c *.h

rebuild: clean all

clean:
	rm -rf report_html
	rm -rf ./tests/*.info ./tests/*.out
	rm -f *.o *.a tests/unit_tests *.gcno *.gcda tests/*.gcno tests/*.gcda *.out *.gcov
	rm -f $(OBJ_FILES)
