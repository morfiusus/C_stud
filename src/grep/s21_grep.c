#include "s21_grep.h"

int main(int argc, char *argv[]) {
  activate flag = {0};
  regex_t sample[SIZE_SAMPLES];
  int sample_pointer = 0;
  int regflags = REG_EXTENDED;
  if (input(argc, argv, &flag, sample, &sample_pointer, &regflags)) {
    output(argc, argv, flag, sample, &sample_pointer, regflags);
  }
  return 0;
}

int input(int argc, char *argv[], activate *flag, regex_t sample[],
          int *sample_pointer, int *regflags) {
  int opt;
  int result = 1;
  opterr = 0;
  if (argc <= 2) {
    fprintf(stderr, "Usage: %s [OPTIONS] PATTERN [FILE]...\n", argv[0]);
    result = 0;
  }
  while ((opt = getopt(argc, argv, "e:f:ivocshln")) != -1) {
    if (opt == 'i') {
      *regflags |= REG_ICASE;
    }
  }
  opterr = 1;
  optind = 1;
  while ((opt = getopt(argc, argv, "e:f:ivoclshn")) != -1) {
    switch (opt) {
      case 'e':
        if (!optarg) {
          fprintf(stderr, "grep: option requires an argument -- 'e'\n");
          result = 0;
        }
        regcomp(&sample[*sample_pointer], optarg, *regflags);
        (*sample_pointer)++;
        flag->e = 1;
        break;
      case 'f':
        result = flag_f_on(sample_pointer, sample, flag, regflags);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case '?':
        result = 0;
        break;
    }
  }
  return result;
}

int flag_f_on(int *sample_pointer, regex_t sample[], activate *flag,
              int *regflags) {
  int result = 1;
  flag->f = 1;
  FILE *flag_f = fopen(optarg, "r");
  if (!flag_f) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
    result = 0;
  }
  char str[SIZE_STR];
  while (fgets(str, sizeof(str), flag_f) && *sample_pointer < SIZE_SAMPLES) {
    size_t len = strlen(str);
    if (len == 0) {
      continue;
    }
    if (len > 0 && str[len - 1] == '\n') {
      str[len - 1] = '\0';
    }
    if (strlen(str) > 0) {
      int ret = regcomp(&sample[*sample_pointer], str, *regflags);
      if (ret != 0) {
        char errbuf[100];
        regerror(ret, &sample[*sample_pointer], errbuf, sizeof(errbuf));
        fprintf(stderr, "grep: %s: %s\n", optarg, errbuf);
        result = 0;
        break;
      }
      (*sample_pointer)++;
    }
  }
  fclose(flag_f);
  return result;
}

void output(int argc, char *argv[], const activate flag, regex_t sample[],
            int *sample_pointer, int regflags) {
  int count_fl = argc - optind;
  if (!flag.e && !flag.f && count_fl > 0) {
    count_fl--;
  }
  int count_l = 0;
  int count_valid_str = 0;
  for (int j = optind; j < argc; j++) {
    if (!flag.e && !flag.f && j == optind) {
      regcomp(&sample[*sample_pointer], argv[j], regflags);
      (*sample_pointer)++;
    } else {
      flag_e_on(argv[j], flag, sample, sample_pointer, count_fl, &count_l,
                &count_valid_str);
    }
  }

  for (int i = 0; i < *sample_pointer; i++) {
    regfree(&sample[i]);
  }
}
void processing_str(char *argv, const activate flag, FILE *file,
                    int *sample_pointer, regex_t sample[], int count_fl,
                    int *count_l, int *count_valid_str) {
  char str[SIZE_STR];
  int num_str = 0;
  while (fgets(str, sizeof(str), file)) {
    num_str++;
    int res = 0;
    regmatch_t match;
    for (int s = 0; s < *sample_pointer; s++) {
      char *sample_pos = str;
      while (*sample_pos != '\0') {
        if (regexec(&sample[s], sample_pos, 1, &match, 0) == 0) {
          res = 1;
          flag_o_on(flag, match, res, argv, count_l, count_fl, count_valid_str,
                    num_str, sample_pos);
          sample_pos += match.rm_eo;
        } else {
          sample_pos++;
        }
      }
      if (flag.o && res) {
        (*count_valid_str)++;
      }
    }
    if (!flag.o) {
      if (res && !flag.v) {
        printing(argv, str, flag, count_fl, count_l, count_valid_str, &num_str);
      } else if (!res && flag.v) {
        printing(argv, str, flag, count_fl, count_l, count_valid_str, &num_str);
      }
    }
  }
}
void flag_e_on(char *argv, const activate flag, regex_t sample[],
               int *sample_pointer, int count_fl, int *count_l,
               int *count_valid_str) {
  FILE *file = fopen(argv, "r");
  if (!file) {
    if (!flag.s) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv);
    }
  } else {
    processing_str(argv, flag, file, sample_pointer, sample, count_fl, count_l,
                   count_valid_str);
    flag_c_on(argv, count_fl, count_valid_str, flag);
    *count_valid_str = 0;
    *count_l = 0;
    fclose(file);
  }
}
void printing(char *argv, char *str, const activate flag, int count_fl,
              int *count_l, int *count_valid_str, int *num_str) {
  if (!flag.o) {
    (*count_valid_str)++;
  }
  if (!flag_l_on(argv, count_l, flag, *count_valid_str)) {
    if (!flag.c) {
      if (count_fl > 1 && !flag.h) {
        printf("%s:", argv);
      }
      flag_n_on(flag, num_str);
      printf("%s", str);
      if (flag.o) {
        putchar('\n');
      }
    }
  }
}
int flag_l_on(char *argv, int *count_l, const activate flag,
              int count_valid_str) {
  int result = 0;
  if (flag.l) {
    if (count_valid_str == 1) {
      *count_l = 1;
      printf("%s\n", argv);
    }
    result = 1;
  }
  return result;
}
void flag_c_on(char *argv, int count_fl, int *count_valid_str,
               const activate flag) {
  if (flag.c && !flag.l) {
    if (count_fl > 1 && !flag.h) {
      printf("%s:", argv);
    }
    printf("%d\n", *count_valid_str);
  }
}
void flag_n_on(const activate flag, int *num_str) {
  if (flag.n) {
    printf("%d:", *num_str);
  }
}
void flag_o_on(const activate flag, regmatch_t match, int res, char *argv,
               int *count_l, int count_fl, int *count_valid_str, int num_str,
               char *sample_pos) {
  if (flag.o) {
    int start = match.rm_so;
    int end = match.rm_eo;
    size_t len = end - start;

    char match_str[SIZE_STR];
    strncpy(match_str, sample_pos + start, len);
    match_str[len] = '\0';
    if (res && !flag.v) {
      printing(argv, match_str, flag, count_fl, count_l, count_valid_str,
               &num_str);
    }
  }
}
