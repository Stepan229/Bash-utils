CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clang:
	cp ../../materials/linters/.clang-format

check:
	clang-format -n *.c *.h

format:
	clang-format -i *.c *.h

clean:
	rm -rf *.o
