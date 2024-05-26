
#ifndef NEW_GREP_H
#define NEW_GREP_H
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
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
  int count_files;
} options;

int parse_options(int argc, char **argv, options *opt, char **parttern);
void set_pattern(options *opt, char **parttern, const char *optarg);
void set_pattern_from_file(options *opt, char **parttern, const char *filename);
void grep(const char *filename, options opt, const char *pattern);
void print_pre(options opt, const char *filename, int line_number);
char *copy_str(const char *str);
#endif