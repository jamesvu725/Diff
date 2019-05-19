#ifndef para_h
#define para_h

#include <stdio.h>

typedef struct para para;
struct para {
  char** base;
  int filesize;
  int start;
  int stop;
  char* firstline;   // DEBUG only
  char* secondline;
};

para* para_make(char* base[], int size, int start, int stop);
para* para_first(char* base[], int size);
para* para_next(para* p);
size_t para_filesize(para* p);
size_t para_size(para* p);
char** para_base(para* p);
char* para_info(para* p);
int   para_equal(para* p, para* q, int ignorecase);

void para_print(para* p, void (*fp)(const char*));
void para_printfile(char* base[], int count, void (*fp)(const char*));
void para_printboth(para* p, para* q);
void para_printnormal(para* p, para* q, void(*fp)(const char*, const char*));

#endif /* para_h */
