#include "s21_cat.h"

int main(int argc, char *argv[]) {
  activate flag = {0};
  if (input(argc, argv, &flag)) {
    output(argc, argv, flag);
  }
  return 0;
}

int checkValidFiles(int argc, char *argv[]) {
  int result = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      FILE *file = fopen(argv[i], "rd");
      if (file == NULL) {
        printf("cat: %s: argv: No such file or directory\n", argv[i]);
        result = 0;
      } else {
        fclose(file);
      }
    }
  }
  return result;
}

int activatedOptions(int argc, char *argv[], activate *flag) {
  int opt;
  int result = 1;

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, "beEstTn", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'E':
        flag->E = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'T':
        flag->T = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case '?':
        printf(
            "cat: unrecognized option '-%c'\nTry 'cat --help' for more "
            "information.\n",
            opt);
        result = 0;
        break;
    }
  }
  return result;
}
int input(int argc, char *argv[], activate *flag) {
  int result;
  result = checkValidFiles(argc, argv) * activatedOptions(argc, argv, flag);
  return result;
}

void output(int argc, char *argv[], const activate flag) {
  int num_str = 0, null_pointer = 0;
  for (int j = 1; j < argc; j++) {
    if (argv[j][0] != '-') {
      FILE *file = fopen(argv[j], "rb");
      char str[SIZE_STR];
      while (fgets(str, sizeof(str), file)) {
        if (flag.s) {
          if (str[0] == '\n') {
            null_pointer++;
            if (null_pointer > 1) {
              continue;
            }
          } else {
            null_pointer = 0;
          }
        }
        if (flag.n || flag.b) {
          if (flag.b) {
            if (str[0] != '\n') {
              num_str++;
              printf("%6d\t", num_str);
            }
          } else {
            num_str++;
            printf("%6d\t", num_str);
          }
        }
        if (flag.t || flag.T || flag.e || flag.E) {
          flag_for_char(str, flag);
        } else {
          printf("%s", str);
        }
      }
      fclose(file);
    }
  }
}

void flag_v(unsigned char ch) {
  if (ch >= 32 && ch <= 126) {
    putchar(ch);
  }
  if (ch < 32) {
    putchar('^');
    putchar(ch + 64);
  }
  if (ch == 127) {
    putchar('^');
    putchar('?');
  }
  if (ch > 127) {
    putchar('M');
    putchar('-');
    flag_v(ch - 128);
  }
}
void flag_for_char(char *str, const activate flag) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (flag.t || flag.T) {
      if (str[i] == '\t') {
        putchar('^');
        putchar('I');
        continue;
      }
    }
    if (flag.e || flag.E) {
      if (str[i] == '\n') {
        putchar('$');
        putchar('\n');
        continue;
      }
    }
    if (flag.v && str[i] != 9 && str[i] != 10) {
      flag_v(str[i]);
    } else {
      putchar(str[i]);
    }
  }
}