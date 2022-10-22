#include "s21_cat.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  struct flag FLAG;
  char pars = 0;
  if (argc > 1) {
    free_flag(&FLAG);
    pars = parser_option(argc, argv, &FLAG);
    if (FLAG.b) FLAG.n = 0;
    if (pars) {
      error(pars);
    } else {
      print(argc, argv, FLAG);
    }
  } else if (argc == 1) {
    char ch = 1;
    while (ch) {
      ch = getc(stdin);
      putchar(ch);
    }
  }

  return (pars) ? 1 : 0;
}

void free_flag(struct flag *FLAG) {
  FLAG->b = 0;
  FLAG->e = 0;
  FLAG->n = 0;
  FLAG->s = 0;
  FLAG->t = 0;
  FLAG->v = 0;
}

void error(char pars) {
  printf("s21_cat: illegal option -- %c\nusage: s21_cat [-benstv] [file ...]",
         pars);
}

char parser_option(int argc, char **argv, struct flag *FLAG) {
  int err = 0;
  char ret = 0;
  if (argc > 1) {
    for (int i = 1; i < argc && !err; i++) {
      int flag_word = 0;
      for (int j = 1; j < (int)strlen(argv[i]) && !flag_word && !err; j++) {
        if (argv[i][0] == '-') {
          switch (argv[i][j]) {
            case ('b'): {
              FLAG->b = 1;
              break;
            }
            case ('e'): {
              FLAG->e = 1;
              FLAG->v = 1;
              break;
            }
            case ('n'): {
              FLAG->n = 1;
              break;
            }
            case ('s'): {
              FLAG->s = 1;
              break;
            }
            case ('t'): {
              FLAG->t = 1;
              FLAG->v = 1;
              break;
            }
            case ('v'): {
              FLAG->v = 1;
              break;
            }
            /*GNU*/
            case ('E'): {
              FLAG->e = 1;
              break;
            }
            case ('T'): {
              FLAG->t = 1;
              break;
            }
            case ('-'): {
              if (strcmp(argv[i], "--number-nonblank") == 0) FLAG->b = 1;
              if (strcmp(argv[i], "--number") == 0) FLAG->n = 1;
              if (strcmp(argv[i], "--squeeze-blank") == 0) FLAG->s = 1;
              if (!FLAG->b && !FLAG->n && !FLAG->s) {
                ret = argv[i][j];
                err = 1;
              }
              flag_word = 1;
              break;
            }
            default: {
              ret = argv[i][j];
              err = 1;
              break;
            }
          }
        }
      }
    }
  }
  return (err) ? ret : 0;
}

char *print(int argc, char **argv, struct flag FLAG) {
  FILE *file;
  char ch;
  int num = 1;
  int enter_1 = 1;
  int enter_2 = 1;
  int dop_check = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      if ((file = fopen(argv[i], "r"))) {
        for (int j = 0; (ch = fgetc(file)) != EOF; j++) {
          if (FLAG.s && enter_1 && dop_check && ch == '\n') continue;
          if (FLAG.b && enter_2 && ch != '\n') printf("%6d\t", num++);
          if (FLAG.n && enter_1) printf("%6d\t", num++);
          if (FLAG.t && ch == '\t') {
            printf("^");
            ch = 'I';
          }
          if (FLAG.e && ch == '\n') printf("$");
          if (FLAG.v && ((ch >= 0 && ch <= 8) || (ch >= 11 && ch <= 31))) {
            printf("^");
            ch += '@';
          }
          if (FLAG.v && ch < 0) {
            printf("M-^");
            ch -= '@';
          }
          if (FLAG.v && ch == 127) {
            printf("^");
            ch = '?';
          }
          enter_1 = (ch == '\n') ? 1 : 0;
          dop_check = (ch == '\n' && enter_2) ? 1 : 0;
          enter_2 = (ch == '\n' && enter_1) ? 1 : 0;
          putchar(ch);
        }
        fclose(file);
      } else {
        printf("s21_cat: %s: %s", argv[i], strerror(errno));
      }
    }
  }
  return 0;
}
