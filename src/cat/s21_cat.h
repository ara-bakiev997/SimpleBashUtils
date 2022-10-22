#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

struct flag {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

void free_flag(struct flag *FLAG);
void error(char pars);
char parser_option(int argc, char **argv, struct flag *FLAG);
char *print(int argc, char **argv, struct flag FLAG);

#endif  // SRC_CAT_S21_CAT_H_
