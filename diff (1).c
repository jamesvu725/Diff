//
//  main.c
//  diff
//
//  Created by William McCarthy on 4/29/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define BUFLEN 256
#define MAXSTRINGS 1024

FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
  return f;
}

int main(int argc, const char * argv[]) {
  char buf[BUFLEN];
  char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
  
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));
  
  if (argc != 3) { fprintf(stderr, "Usage: ./diff file1 file2\n");  exit(ARGC_ERROR); }

  FILE *fin1 = openfile(argv[1], "r");
  FILE *fin2 = openfile(argv[2], "r");
  
  int count1 = 0, count2 = 0;
  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) {
    strings1[count1++] = strdup(buf);
  }
  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) {
    strings2[count2++] = strdup(buf);
  }

  for (int i = 0; i < count1 && strcmp(strings1[i], "\n") != 0; ++i) { printf("< %s", strings1[i]); }
  for (int i = 0; i < count2 && strcmp(strings2[i], "\n") != 0; ++i) { printf("%50s %s", ">", strings2[i]); }

  fclose(fin1);
  fclose(fin2);
  return 0;
}
