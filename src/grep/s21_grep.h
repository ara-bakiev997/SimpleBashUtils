#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

struct flag {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int count_file;
  int count_pattern;
  char pattern[100][1024];
  char file[100][1024];
};

void free_flag(struct flag *FLAG);
void in_stdin();
void error_pars(char pars);
void error_in();
int parser_option(int argc, char **argv, struct flag *FLAG);
void open_f(struct flag *FLAG, char *argv);
void print_all(struct flag FLAG);

#endif  // SRC_GREP_S21_GREP_H_
