CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all : s21_grep

s21_grep: s21_grep.o
	$(CC) -o s21_grep s21_grep.o

s21_grep.o : s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) -c s21_grep.c

clean:
	rm -f *.o s21_grep

rebuild: clean all