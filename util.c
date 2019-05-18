#include <string.h>
#include <stdlib.h>
#include "util.h"

#define BUFLEN 256

// returns no if 0 and yes if other
char* yesorno(int condition) { return condition == 0 ? "no" : "YES"; }
// open file and returns file pointer
FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
  return f;
}
// prints out 10 lines of =
void printline(void) {
  for (int i = 0; i < 10; ++i) { printf("========"); }
  printf("\n");
}
// prints out left side
void printleft(const char* left) {
  char buf[BUFLEN];
  // copy left of buffer
  strcpy(buf, left);
  // len is buflen -1
  int j = 0, len = (int)strlen(buf) - 1;
  // if j is greater or equal to 0, fill in buffer with ' '
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '<';
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}
// prints out right side
void printright(const char* right) {
  if (right == NULL) { return; }
  printf("%50s %s", ">", right);
}
// prints out both side
void printboth(const char* left, const char* right) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 49 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '\0';
  printf("%-50s %s", buf, right);
}
// print out both with change
void printchange(const char* left, const char* right) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '|';
  buf[len + j++] = '\0';
  printf("%-50s %s", buf, right);
}

void printnormaladd(const char* left, const char* right) {
  if (right == NULL) { return; }
  printf("> %s", right);
}

void printnormalchange(const char* left, const char* right) {
  printnormaladd(left, right);
  printf("---\n");
  printnormaldelete(left, right);
}

void printnormaldelete(const char* left, const char* right) {
  if (left == NULL) { return; }
  printf("< %s", left);
}
