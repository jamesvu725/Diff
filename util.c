#include <string.h>
#include <stdlib.h>
#include "util.h"

#define BUFLEN 256

FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
  return f;
}

void printleft(const char* left) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '<';
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}

void printright(const char* right) {
  if (right == NULL) { return; }
  printf("%50s %s", ">", right);
}

void printboth(const char* left, const char* right) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 49 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '\0';
  printf("%-50s %s", buf, right);
}

void printchange(const char* left, const char* right) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '|';
  buf[len + j++] = '\0';
  printf("%-50s %s", buf, right);
}

void printleftcol(const char* left) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '(';
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}

void printnormaladd(const char* right) {
  if (right == NULL) { return; }
  printf("> %s", right);
}

void printnormaldelete(const char* left) {
  if (left == NULL) { return; }
  printf("< %s", left);
}
