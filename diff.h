#ifndef diff_h
#define diff_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

#define MAXSTRINGS 1024
#define MAXPARAS 4096

#define BUFLEN 256

typedef struct para para;
struct para {
  char** base;
  int filesize;
  int start;
  int stop;
};

char buf[BUFLEN];
char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;
int count1 = 0, count2 = 0;
const char* files[2] = { NULL, NULL };

void version(void);
void loadfiles(const char* filename1, const char* filename2);
void diff_output_conflict_error(void);
void setoption(const char* arg, const char* s, const char* t, int* value);
void init_options_files(int argc, const char* argv[]);

void print_normal(para* p, para* q);
int print_brief(para* p, para* q);
int print_identical(para* p, para* q);
void print_sidebyside(para* p, para* q);
void print_context(para* p, para* q);
void print_unified(para* p, para* q);

para* para_make(char* base[], int size, int start, int stop);
para* para_first(char* base[], int size);
para* para_next(para* p);
size_t para_filesize(para* p);
size_t para_size(para* p);
char** para_base(para* p);
char* para_info(para* p);
int   para_equal(para* p, para* q, int ignorecase);

void para_print(para* p, void (*fp)(const char*));
void para_printboth(para* p, para* q, int suppresscommon, int showleftcolumn);

FILE* openfile(const char* filename, const char* openflags);

void printleft(const char* left);
void printright(const char* right);
void printboth(const char* left, const char* right);
void printchange(const char* left, const char* right);
void printleftcol(const char* left);

void printnormaladd(const char* right);
void printnormaldelete(const char* left);

#endif /* diff_h */
