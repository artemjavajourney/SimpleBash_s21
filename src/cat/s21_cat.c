#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
  int flag_error;
} flags;

int parser(int argc, char *argv[], flags *flag);
void cat(char *path, flags flag);
void print_file(FILE *file, flags flag);

int main(int argc, char *argv[]) {
  int number_file;
  flags flag = {0};

  number_file = parser(argc, argv, &flag);

  if (flag.flag_error == 1) {
    fprintf(stderr, "illegal option flag");
  } else {
    do {
      char *path = argv[number_file];
      cat(path, flag);
      number_file++;

    } while (number_file < argc);
  }

  return 0;
}

int parser(int argc, char *argv[], flags *flag) {
  const struct option long_flags[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "+beEnstvT?", long_flags, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flag->flag_b = 1;
        break;
      case 'e':
        flag->flag_e = 1;
        flag->flag_v = 1;
        break;
      case 'E':
        flag->flag_e = 1;
        break;
      case 'n':
        flag->flag_n = 1;
        break;
      case 's':
        flag->flag_s = 1;
        break;
      case 't':
        flag->flag_t = 1;
        flag->flag_v = 1;
        break;
      case 'T':
        flag->flag_t = 1;
        break;
      case 'v':
        flag->flag_v = 1;
        break;
      case '?':
        flag->flag_error = 1;
        break;
    }
  }
  return optind;
}

void cat(char *path, flags flag) {
  if (access(path, F_OK) == 0) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
      print_file(file, flag);
      fclose(file);
    } else
      fprintf(stderr, "Permision");
  } else {
    if (path != NULL) {
      fprintf(stderr, "cat: %s: No such file or directory", path);
    } else
      print_file(stdin, flag);
  }
}

void print_file(FILE *file, flags flag) {
  char ch, prev;
  int current_line = 1;
  int squeeze = 0;

  for (prev = '\n'; (ch = getc(file)) != EOF; prev = ch) {
    if (flag.flag_s == 1 && (ch == '\n' && prev == '\n')) {
      if (squeeze == 1) continue;

      squeeze++;
    } else {
      squeeze = 0;
    }

    if (flag.flag_b == 1) {
      if (prev == '\n' && ch != '\n') {
        printf("%*d\t", 6, current_line);
        current_line++;
      }
    }

    if (flag.flag_n == 1 && flag.flag_b == 0 && prev == '\n') {
      printf("%*d\t", 6, current_line);
      current_line++;
    }

    if (flag.flag_e == 1 && ch == '\n') {
      if (flag.flag_b == 1 && prev == '\n') printf("%*c", 7, '\t');

      putchar('$');
    }

    if (flag.flag_t == 1 && ch == '\t') {
      printf("^");
      ch = 'I';
    }

    if (flag.flag_v == 1) {
      if ((ch >= 0 && ch <= 31) && ch != '\t' && ch != '\n') {
        printf("^");
        ch = ch + 64;
      } else if (ch == 127) {
        printf("^");
        ch = ch - 64;
      }
    }
    putchar(ch);
  }
}