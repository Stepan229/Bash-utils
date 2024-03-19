#include "s21_grep.h"

int main(int argc, char *argv[]) { s21_grep(argc, argv); }

void parseParams(int argc, char **argv, Flags *conf, char *template) {
  int flag = 0;
  opterr = 0;
  // for (int sym = 0; (sym = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
  for (int sym = 0;
       (sym = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != (-1);) {
    // printf("%d", optind);
    switch (sym) {
      case 'i':
        conf->i = REG_ICASE;
        break;
      case 'v':
        conf->v = 1;
        break;
      case 'c':
        conf->c = 1;
        break;
      case 'l':
        conf->l = 1;
        break;
      case 'n':
        conf->n = 1;
        break;
      case 'h':
        conf->h = 1;
        break;
      case 's':
        conf->s = 1;
        break;
      case 'o':
        conf->o = 1;
        break;
      case 'e':
        catStrTemplate(template);
        conf->e = 1;
        break;
      case 'f':
        parseFileTemp(template, optarg);
        conf->f = 1;
        break;
      default:
        flag = 1;
        break;
    }
  }

  isNoFlag(conf, argv, template);
  if (flag == 1) fprintf(stderr, "There are incorrect flags\n");
  setupNumbersFiles(conf, argc);
}

void setupNumbersFiles(Flags *conf, int argc) {
  conf->count_files = argc - optind - 1;
  if (conf->l == 1) {
    conf->o = 0;
    conf->n = 0;
  }
}

void isNoFlag(Flags *conf, char **argv, char *template) {
  if (conf->e == 0 && conf->f == 0) {
    template = strcat(template, argv[optind]);
    optind++;
    if (strlen(template) != 0) conf->e = 1;
  }
  if (conf->l) conf->c = 0;
  if (conf->c) {
    conf->o = 0;
  }
}

void catStrTemplate(char *template) {
  if (strlen(template) != 0) template = strcat(template, "|");
  template = strcat(template, optarg);
}

void s21_grep(int argc, char *argv[]) {
  char template[5000] = "";
  Flags conf = {0};
  parseParams(argc, argv, &conf, template);
  for (; optind < argc;) {
    parseFile(template, &conf, argv[optind]);
    conf.count_matches = 0;
    conf.number_line = 0;
    optind++;

    // if (optind != argc) хз чё это
    //   printf("\n");
  }
  // parseFile(&conf, &template);
}

void parseFile(char *template, Flags *conf, char *nameFile) {
  char *line = NULL;
  size_t n;
  ssize_t len;
  FILE *file = fopen(nameFile, "r");
  if (file == NULL) {
    if (!conf->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", nameFile);
  } else {
    while ((len = getline(&line, &n, file)) != -1) {
      if (conf->e || conf->f) {
        flagE(template, conf, line, nameFile, len);
      }
      if (conf->n) conf->number_line++;
      if (conf->l && conf->count_matches >= 1) break;
    }
    if (conf->c && conf->count_files > 0 && !conf->h)
      printf("%s:%d\n", nameFile, conf->count_matches);
    else if (conf->c && (conf->count_files == 0 || conf->h))
      printf("%d\n", conf->count_matches);
    if (conf->l && conf->count_matches > 0) {
      printf("%s\n", nameFile);
    }
    free(line);
    fclose(file);
  }
}

int checkI(regex_t regex, Flags *conf, char *string, regmatch_t *pm) {
  int match = 0;
  match = regexec(&regex, string, 1, pm, 0);
  if (conf->v == 1 && match == 1)
    match = 0;
  else if (conf->v == 1 && match == 0)
    match = 1;
  return match;
}

void flagE(char *template, Flags *conf, char *string, char *file, ssize_t len) {
  regex_t regex;
  regmatch_t pm = {0};
  int match = 0;
  regcomp(&regex, template, (conf->i | REG_EXTENDED));  //сделать проверку
  match = checkI(regex, conf, string, &pm);
  if (!match) {
    // printf("|%d%d%d%d%d|", conf->e, conf->l, conf->c, conf->o, conf->h);
    if (conf->c || conf->l) conf->count_matches++;
    if (conf->count_files > 0 && !conf->h && !conf->c && !conf->l)
      printf("%s:", file);
    if (conf->n && !conf->c && !conf->l) printf("%d:", (conf->number_line + 1));
    if (conf->o) flagO(string, &pm);
    if (!conf->o && !conf->l && !conf->c && (conf->h || conf->e || conf->f))
      printf("%s", string);
    if (string[len - 1] != '\n' && !conf->o && !conf->c && !conf->l) {
      printf("\n");
    }
  }
  regfree(&regex);
}

void parseFileTemp(char *template, char *nameFile) {
  char *line = NULL;
  size_t n;
  ssize_t len;
  FILE *file = fopen(nameFile, "r");
  if (file == NULL) {
    printf("grep: %s: No such file or directory\n", nameFile);
  } else {
    while ((len = getline(&line, &n, file)) != -1) {
      if (strlen(template) != 0) template = strcat(template, "|");
      if (strstr(line, " ") != NULL) {
        printf("%s\n", "sac");
        template = strcat(template, "\\s");
      }
      if (line[len - 1] == '\n') line[len - 1] = '\0';
      template = strcat(template, line);
    }
    free(line);
  }
  fclose(file);
}

void flagO(char *string, regmatch_t *pm) {
  for (int i = pm->rm_so; i < pm->rm_eo; i++) {
    printf("%c", string[i]);
  }
  printf("\n");
}
