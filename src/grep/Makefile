CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: s21_grep 

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clang:
	cp ../../materials/linters/.clang-format

check:
	clang-format -n *.c *.h

format:
	clang-format -i *.c *.h

install: s21_grep
	cp s21_grep test/

clean:
	rm -rf *.o
