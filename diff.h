#ifndef diff_h
#define diff_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "para.h"
#include "util.h"

#define ARGC_ERROR 1
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

#define MAXSTRINGS 1024
#define MAXPARAS 4096

#define HASHLEN 200

char buf[BUFLEN];
char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;
int count1 = 0, count2 = 0;
const char* files[2] = { NULL, NULL };

void version(void);
void loadfiles(const char* filename1, const char* filename2);
void print_option(const char* name, int value);
void diff_output_conflict_error(void);
void setoption(const char* arg, const char* s, const char* t, int* value);
void showoptions(const char* file1, const char* file2);
void init_options_files(int argc, const char* argv[]);

void print_normal(para* p, para* q);
int print_brief(para* p, para* q);
void print_identical(para* p, para* q);
void print_sidebyside(para* p, para* q);
void print_context(para* p, para* q);
void print_unified(para* p, para* q);

#endif /* diff_h */
