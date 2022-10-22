#include "s21_grep.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct flag FLAG;
  if (argc > 1) {
    free_flag(&FLAG);
    if (!parser_option(argc, argv, &FLAG))
      print_all(FLAG);
    else if (!FLAG.s)
      printf("parser_option error");
  } else if (argc == 1) {
    error_in();
  }
  return 0;
}

void free_flag(struct flag *FLAG) {
  FLAG->e = 0;
  FLAG->i = 0;
  FLAG->v = 0;
  FLAG->c = 0;
  FLAG->l = 0;
  FLAG->n = 0;
  FLAG->h = 0;
  FLAG->s = 0;
  FLAG->f = 0;
  FLAG->o = 0;
  FLAG->count_file = 0;
  FLAG->count_pattern = 0;
}

void in_stdin() {
  char ch = 1;
  while (ch) {
    ch = getc(stdin);
    putchar(ch);
  }
}

void error_in() {
  printf(
      "usage: s21_grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
      "[-C[num]]\n"
      "        [-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
      "        [--context[=num]] [--directories=action] [--label] "
      "[--line-buffered]\n"
      "        [--null] [pattern] [file ...]");
}

int parser_option(int argc, char **argv, struct flag *FLAG) {
  int ret = 0;
  int flag_F_E = 0;
  int have_pattern = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strchr(argv[i], 'e') || strchr(argv[i], 'f')) flag_F_E = 1;
    }
  }
  for (int i = 1; i < argc; i++) {
    for (size_t j = 0; j < strlen(argv[i]); j++) {
      if (argv[i][0] == '-') {
        switch (argv[i][j]) {
          case ('i'): {
            FLAG->i = 1;
            break;
          }
          case ('v'): {
            FLAG->v = 1;
            break;
          }
          case ('c'): {
            FLAG->c = 1;
            break;
          }
          case ('l'): {
            FLAG->l = 1;
            break;
          }
          case ('n'): {
            FLAG->n = 1;
            break;
          }
          case ('h'): {
            FLAG->h = 1;
            break;
          }
          case ('s'): {
            FLAG->s = 1;
            break;
          }
          case ('o'): {
            FLAG->o = 1;
            break;
          }
          case ('e'): {
            FLAG->e = 1;
            strcpy(FLAG->pattern[FLAG->count_pattern++], argv[++i]);
            memset(argv[i], '\0', strlen(argv[i]));
            break;
          }
          case ('f'): {
            FLAG->f = 1;
            open_f(FLAG, argv[++i]);
            memset(argv[i], '\0', strlen(argv[i]));
            break;
          }
        }
      } else {
        if (!flag_F_E && !have_pattern) {
          strcpy(FLAG->pattern[FLAG->count_pattern++], argv[i]);
          have_pattern = 1;
          break;
        }
        if (have_pattern) {
          strcpy(FLAG->file[FLAG->count_file++], argv[i]);
          break;
        } else {
          strcpy(FLAG->file[FLAG->count_file++], argv[i]);
          break;
        }
      }
    }
  }
  if (argc == 2 && !FLAG->i && !FLAG->e && !FLAG->i && !FLAG->v && !FLAG->c &&
      !FLAG->l && !FLAG->n && !FLAG->h && !FLAG->s && !FLAG->f && !FLAG->o) {
    ret = 1;
    in_stdin();
  }
  return ret;
}

void open_f(struct flag *FLAG, char *argv) {
  FILE *p_file;
  size_t n;
  char *str = NULL;
  if ((p_file = fopen(argv, "r"))) {
    size_t open;
    for (int i = 0; ((int)(open = getline(&str, &n, p_file)) != EOF); i++) {
      if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
      strcpy(FLAG->pattern[FLAG->count_pattern++], str);
    }
    free(str);
    fclose(p_file);
  } else {
    if (!FLAG->s) printf("s21_grep: %s: %s", argv, strerror(errno));
  }
}

void print_all(struct flag FLAG) {
  if (FLAG.l) {
    FLAG.n = 0;
    FLAG.o = 0;
  }
  if (FLAG.c) {
    FLAG.n = 0;
    FLAG.o = 0;
  }
  if (FLAG.v) FLAG.o = 0;
  char *str = NULL;
  regex_t preg;
  int flag_i = 0;
  if (FLAG.i != 0) flag_i |= REG_ICASE;
  regmatch_t match[1];
  for (int i = 0; i < FLAG.count_file; i++) {
    FILE *fileArray;
    if ((fileArray = fopen(FLAG.file[i], "r"))) {
      size_t n, open;
      int c_num = 0;
      for (int j = 0; ((int)(open = getline(&str, &n, fileArray)) != EOF);
           j++) {
        if ((str[strlen(str) - 1] == '\n')) str[strlen(str) - 1] = '\0';
        int succsess = 0;
        int match_in = 0;
        for (int count = 0; count < FLAG.count_pattern; count++) {
          if (regcomp(&preg, FLAG.pattern[count], flag_i) == 0) {
            succsess = regexec(&preg, str, 1, match, 0);
            if (!succsess) match_in = 1;
          }
          regfree(&preg);
        }
        if (FLAG.v == !match_in) {
          c_num++;
          if (FLAG.count_file > 1 && !FLAG.l && !FLAG.h && !FLAG.c)
            printf("%s:", FLAG.file[i]);
          if (FLAG.n) {
            printf("%d:", j + 1);
          }
          if (FLAG.o) {
            for (int o = match[0].rm_so; o < match[0].rm_eo; o++) {
              printf("%c", str[o]);
            }
            printf("\n");
          }
          if (!FLAG.l && !FLAG.c && !FLAG.o) printf("%s\n", str);
        }
      }
      if (FLAG.l || FLAG.c) {
        if (FLAG.c) {
          if (FLAG.l) {
            if (FLAG.count_file > 1 && !FLAG.h) printf("%s:", FLAG.file[i]);
            if (c_num) printf("%d\n", 1);
            if (!c_num) printf("%d\n", 0);
          } else {
            if (FLAG.count_file > 1 && !FLAG.h) printf("%s:", FLAG.file[i]);
            printf("%d\n", c_num);
          }
        }
        if (FLAG.l) {
          if (c_num) printf("%s\n", FLAG.file[i]);
        }
      }
      fclose(fileArray);
    } else {
      if (!FLAG.s) printf("s21_grep: %s: %s", FLAG.file[i], strerror(errno));
    }
  }
  free(str);
}
