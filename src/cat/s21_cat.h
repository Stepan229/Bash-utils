#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;

int inputArgsParse(int argc, char *argv[], Flags *flags, int *numFlags);
void parsNameFile(int argc, char *argv[], int numFlags, Flags *flags);
int parsFile(Flags *flags, FILE *fp);
int parsArg(char *arg, Flags *flags);
int dispFlagV(char symb_char);
void dispFlagET(char *line, int len, Flags *flags, int num);