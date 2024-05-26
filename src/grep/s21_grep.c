#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    options opt = {0};
    char *pattern = NULL;

    int start_index = parse_options(argc, argv, &opt, &pattern);

    for (int i = start_index; i < argc; i++) {
      grep(argv[i], opt, pattern);
    }

    if (pattern) free(pattern);
  }

  return 0;
}

int parse_options(int argc, char **argv, options *opt, char **parttern) {
  int current_opt;
  while ((current_opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) !=
         -1) {
    switch (current_opt) {
      case 'e':
        set_pattern(opt, parttern, optarg);
        break;
      case 'f':
        set_pattern_from_file(opt, parttern, optarg);
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'o':
        opt->o = 1;
        break;
    }
  }

  if (!(*parttern)) {
    set_pattern(opt, parttern, argv[optind++]);
  }
  if (opt->v || opt->l || opt->c) opt->o = 0;
  opt->count_files = argc - optind;
  return optind;
}

void set_pattern(options *opt, char **parttern, const char *optarg) {
  if (!opt->e && !opt->f) {
    *parttern = copy_str(optarg);
    opt->e = 1;
  } else {
    size_t len = *parttern ? strlen(*parttern) : 0;
    *parttern = realloc(*parttern, len + strlen(optarg) + 16);
    if (len > 0) strcat(*parttern, "|");
    strcat(*parttern, "(");
    strcat(*parttern, optarg);
    strcat(*parttern, ")");
  }
}

void set_pattern_from_file(options *opt, char **parttern,
                           const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!opt->s) perror("Error opening file");
    return;
  }

  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, file) != -1) {
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    set_pattern(opt, parttern, line);
  }

  free(line);
  fclose(file);
}

void grep(const char *filename, options opt, const char *pattern) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!opt.s) perror("Error opening file");
    return;
  }

  regex_t regex;
  regcomp(&regex, pattern, opt.i ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);

  char *line = NULL;
  size_t len = 0;
  int line_number = 0;
  int match_count = 0;
  int read = 0;
  while ((read = getline(&line, &len, file)) != -1) {
    line_number++;
    if ((opt.l && match_count)) continue;
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    int match = regexec(&regex, line, 0, NULL, 0) == 0;
    if ((match ^ opt.v)) {
      match_count++;
      if (!opt.c && !opt.l) {
        if (opt.o) {
          regmatch_t pmatch;
          char *start = line;
          while (regexec(&regex, start, 1, &pmatch, 0) == 0) {
            print_pre(opt, filename, line_number);
            printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so),
                   start + pmatch.rm_so);
            start += pmatch.rm_eo;
          }
        } else {
          print_pre(opt, filename, line_number);
          printf("%s", line);
          if (line[read - 1] != '\n') printf("\n");
        }
      }
    }
  }
  if (opt.c) {
    if (opt.count_files > 1 && !opt.h) printf("%s:", filename);
    printf("%d\n", match_count);
  }
  if (opt.l && match_count) {
    printf("%s\n", filename);
  }
  regfree(&regex);
  free(line);
  fclose(file);
}
void print_pre(options opt, const char *filename, int line_number) {
  if (opt.count_files > 1 && !opt.h) printf("%s:", filename);
  if (opt.n) printf("%d:", line_number);
}

char *copy_str(const char *str) {
  if (str == NULL) {
    return NULL;
  }

  size_t len = strlen(str);

  char *copy = (char *)malloc((len + 1) * sizeof(char));
  if (copy == NULL) {
    return NULL;
  }

  strcpy(copy, str);

  return copy;
}