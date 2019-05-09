// array of char*
// fgets to get lines
// strdup to make the memory and copy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define BUFLEN 256

FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {
    printf("can't open '%s'\n", filename);
    exit(1);
  }
  return f;
}

int main(int argc, const char* argv[]) {
  char buf1[BUFLEN], buf2[BUFLEN];
  memset(buf1, 0, sizeof(buf1)); memset(buf2, 0, sizeof(buf2));
  if (argc != 3) { fprintf(stderr, "Usage: ./diff file1 file2\n"); exit(ARGC_ERROR); }

  FILE *fin1 = openfile(argv[1], "r");
  FILE *fin2 = openfile(argv[2], "r");
  int compare = 0;

  while (!feof(fin1) && !feof(fin2) && (compare = strcmp(fgets(buf1, BUFLEN, fin1),
        fgets(buf2, BUFLEN, fin2))) == 0) { }
  if (compare != 0) {
    printf("file1: %s", buf1);
    printf("file2: %s\n", buf2);
  }
  fclose(fin1); fclose(fin2);
  return 0;
}
