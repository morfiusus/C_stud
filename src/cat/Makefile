CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all : s21_cat

s21_cat: s21_cat.o
	$(CC) -o s21_cat s21_cat.o

s21_cat.o : s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) -c s21_cat.c

clean:
	rm -f *.o s21_cat

rebuild: clean all