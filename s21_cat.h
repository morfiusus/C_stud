#ifndef CAT_H
#define CAT_H
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#define SIZE_STR 4096

typedef struct {
  int b;
  int e;
  int E;
  int s;
  int t;
  int T;
  int n;
  int v;
} activate;

int checkValidFiles(int argc, char *argv[]);
int input(int argc, char *argv[], activate *flag);
int activatedOptions(int argc, char *argv[], activate *flag);
void flag_for_char(char *str, const activate flag);
void flag_v(unsigned char ch);
void output(int argc, char *argv[], const activate flag);

#endif