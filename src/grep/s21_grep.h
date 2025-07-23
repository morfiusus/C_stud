#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define SIZE_STR 4096
#define SIZE_SAMPLES 128

typedef struct {
  int e;
  int o;
  int f;
  int h;
  int s;
  int i;
  int v;
  int c;
  int l;
  int n;
} activate;

int input(int argc, char *argv[], activate *flag, regex_t sample[],
          int *sample_pointer, int *regflags);
int flag_f_on(int *sample_pointer, regex_t sample[], activate *flag,
              int *regflags);

void output(int argc, char *argv[], const activate flag, regex_t sample[],
            int *sample_pointer, int regflags);
void processing_str(char *argv, const activate flag, FILE *file,
                    int *sample_pointer, regex_t sample[], int count_fl,
                    int *count_l, int *count_valid_str);

void flag_e_on(char *argv, const activate flag, regex_t sample[],
               int *sample_pointer, int count_fl, int *count_l,
               int *count_valid_str);
void printing(char *argv, char *str, const activate flag, int count_fl,
              int *count_l, int *count_valid_str, int *num_str);

int flag_l_on(char *argv, int *count_l, const activate flag,
              int count_valid_str);
void flag_c_on(char *argv, int count_fl, int *count_valid_str,
               const activate flag);
void flag_n_on(const activate flag, int *num_str);
void flag_o_on(const activate flag, regmatch_t match, int res, char *argv,
               int *count_l, int count_fl, int *count_valid_str, int num_str,
               char *sample_pos);
#endif