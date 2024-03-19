#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define MAX_LENGTH 4000

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
  int number_line;
  int count_matches;
  char template[MAX_LENGTH];
} Flags;

void catStrTemplate(char *template);
void parseParams(int argc, char **argv, Flags *conf, char *template);
void parseFile(char *template, Flags *conf, char *nameFile);
void s21_grep(int argc, char *argv[]);
void setupNumbersFiles(Flags *conf, int argc);
void flagE(char *template, Flags *conf, char *string, char *nameFile,
           ssize_t len);
void parseFileTemp(char *template, char *nameFile);
int checkI(regex_t regex, Flags *conf, char *string, regmatch_t *pm);
void flagO(char *string, regmatch_t *pm);
void checkEndStr(char *string, ssize_t len);
void isNoFlag(Flags *conf, char **argv, char *template);

#endif  // SRC_GREP_S21_GREP_H_