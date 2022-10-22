.PHONY: all test s21_cat s21_grep
# https://habr.com/ru/company/ruvds/blog/586386/
LDFLAGS?=-Ofast
CC=gcc
RM+=-r
override CFLAGS += -Wall -Wextra -Werror  -std=c11
EXE=s21_cat
RUN=$(EXE) s21_*_test.txt

all: $(EXE)

%.o: %.c
	$(CC) -c $(CFLAGS) $^

$(EXE): $(EXE).o
	$(CC) $(LDFLAGS) $^ -o $@

test: $(EXE)
	sh ./s21_*test.sh

check:
	cp ../../materials/linters/CPPLINT.cfg .
	-python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
ifeq ($(shell uname -s), Darwin)
	CK_FORK=no leaks --atExit -- ./$(RUN)
endif
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./$(RUN)

clean:
	ls | grep -v 's21_[a-z_]*[.][c|h|txt|sh]\|Makefile' | xargs $(RM)
# cd liba && ls | grep -v '\w*[.][c|h|txt|sh]\|s21_[a-z_]*[.][c|h]\|Makefile' | xargs $(RM)
