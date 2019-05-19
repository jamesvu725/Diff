#ifndef util_h
#define util_h

#include <stdio.h>
#define BUFLEN 256

char* yesorno(int condition);
FILE* openfile(const char* filename, const char* openflags);

void printleft(const char* left);
void printright(const char* right);
void printboth(const char* left, const char* right);
void printchange(const char* left, const char* right);
void printleftcol(const char* left);

void printline(void);

void printnormaladd(const char* left, const char* right);
void printnormalchange(const char* left, const char* right);
void printnormaldelete(const char* left, const char* right);

#endif /* util_h */
