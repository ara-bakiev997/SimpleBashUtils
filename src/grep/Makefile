.PHONY: all test s21_cat s21_grep
# https://habr.com/ru/company/ruvds/blog/586386/
LDFLAGS?=-Ofast
CC=gcc
RM+=-r
override CFLAGS += -Werror -Wextra -Wall  -std=c11 -D_GNU_SOURCE
EXE=s21_grep
RUN=$(EXE) -e while -e void s21_grep.c Makefile -n -v -c -f s21_pattern.txt

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
	ls |  grep -v 's21_[a-z_]*[.][c|h|txt|sh]\|Makefile' | xargs $(RM)
# cd liba && ls | grep -v '\w*[.][c|h|txt|sh]\|s21_[a-z_]*[.][c|h]\|Makefile' | xargs $(RM)
