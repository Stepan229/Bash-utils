#include "s21_cat.h"
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>

// typedef struct Flags {
//   int b;
//   int e;
//   int n;
//   int s;
//   int t;
//   int v;
// } Flags;

// int inputArgsParse(int argc,char* argv[], char **file,Flags* flags);
// int parsFile(Flags *flags,char **file);
// int parsArg(char* arg, Flags *flags);
// int dispFlagV(char symb_char);
// void dispFlagET(char* line, int len, Flags* flags, int num);

int main(int argc, char *argv[]) {
  int error = 0;
  int numFlags = 0;
  Flags flags = {0, 0, 0, 0, 0, 0};
  error = inputArgsParse(argc, argv, &flags, &numFlags);
  if (error != -10) parsNameFile(argc, argv, numFlags, &flags);
  return 0;
}

int inputArgsParse(int argc, char *argv[], Flags *flags, int *numFlags) {
  int error = 0;
  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "-b") == 0)
      flags->b = 1;
    else if (strcmp(argv[i], "-e") == 0) {
      flags->v = 1;
      flags->e = 1;
    } else if (strcmp(argv[i], "-n") == 0)
      flags->n = 1;
    else if (strcmp(argv[i], "-s") == 0)
      flags->s = 1;
    else if (strcmp(argv[i], "-t") == 0) {
      flags->v = 1;
      flags->t = 1;
    } else if (strcmp(argv[i], "-v") == 0)
      flags->v = 1;
    else if (strcmp(argv[i], "--number-nonblank") == 0)
      flags->b = 1;
    else if (strcmp(argv[i], "-E") == 0)
      flags->e = 1;
    else if (strcmp(argv[i], "--number") == 0)
      flags->n = 1;
    else if (strcmp(argv[i], "--squeeze-blank") == 0)
      flags->s = 1;
    else if (strcmp(argv[i], "-T") == 0)
      flags->t = 1;
    else if (argv[i][0] == '-') {
      parsArg(argv[i], flags);
    } else {
      *numFlags = *numFlags + 1;
    }
  }
  // *file = argv[argc - 1];
  // printf("b=%d e=%d n=%d s=%d t=%d v=%d\n", flags->b, flags->e, flags->n,
  // flags->s, flags->t, flags->v);
  return error;
}

void parsNameFile(int argc, char *argv[], int numFlags, Flags *flags) {
  FILE *pfile = NULL;
  char *file = NULL;
  for (int i = argc - numFlags - 1; i <= argc - 1; i++) {
    file = argv[i];
    pfile = fopen(file, "r");
    if (pfile != NULL) {
      parsFile(flags, pfile);
      fclose(pfile);
    } else
      fprintf(stderr, "cat: %s: No such file or directory\n", file);
  }
}
int parsArg(char *arg, Flags *flags) {
  int sumSimb = 0, len = strlen(arg) - 1, error = 0;
  for (int i = 0; arg[i] != '\0'; i++) {
    if ('b' == arg[i]) {
      flags->b = 1;
      sumSimb++;
    } else if ('e' == arg[i]) {
      flags->v = 1;
      flags->e = 1;
      sumSimb++;
    } else if ('n' == arg[i]) {
      flags->n = 1;
      sumSimb++;
    } else if ('s' == arg[i]) {
      flags->s = 1;
      sumSimb++;
    } else if ('t' == arg[i]) {
      flags->v = 1;
      flags->t = 1;
      sumSimb++;
    }
  }
  if (sumSimb != len) error = -1;

  return error;
}

int parsFile(Flags *flags, FILE *fp) {
  char *line = NULL;
  size_t n;
  ssize_t len = 0;
  int error = 0, n_quantity = 1, nullLine = 0, numLine = 1;
  if (flags->b == 1 && flags->n == 1) flags->n = 0;
  while ((len = getline(&line, &n, fp)) != -1) {
    if (flags->b == 1) {
      if (len != 1) {
        printf("%6d	", n_quantity);
        n_quantity++;
      }
    }
    if (flags->n == 1 && flags->s == 0) {
      printf("%6d	", n_quantity);
      n_quantity++;
    }

    if (flags->s == 1) {
      if (len == 1 && nullLine == 0) {
        dispFlagET(line, len, flags, numLine);
        nullLine = 1;
        numLine++;
      } else if (len > 1 && (nullLine == 1 || nullLine == 0)) {
        dispFlagET(line, len, flags, numLine);
        nullLine = 0;
        numLine++;
      }
    } else if (flags->e == 1 || flags->t == 1 || flags->v == 1)
      dispFlagET(line, len, flags, numLine);
    else
      printf("%s", line);
    // numLinen++;
  }
  free(line);
  return error;
}
void dispFlagET(char *line, int len, Flags *flags, int num) {
  if (flags->n == 1 && flags->s == 1) printf("%6d	", num);
  for (int i = 0; i < len; i++) {
    if (flags->e == 1 && line[i] == '\n')
      printf("$\n");
    else if (flags->t == 1 && line[i] == '\t')
      printf("^I");
    else if (flags->v == 1)
      dispFlagV(line[i]);
    else
      printf("%c", line[i]);
  }
}

int dispFlagV(char symb) {
  int check = 0;
  if ((symb < 32) && (symb >= 0) && (symb != '\n') && (symb != '\t')) {
    printf("^%c", symb + 64);
  } else if (symb == 127) {
    printf("^?");
  } else if ((symb > -128 && symb < -96)) {
    printf("M-^%c", symb + 128 + 64);
  } else
    printf("%c", symb);

  return check;
}
