// diff basic commands
// diff [options] file1 file2
// options to support:
// diff -v aka --version                  (prints version, license, and author (you))
// diff -q aka --brief                    (reports only whether files are different shows nothing if files are identical)
// diff -i aka --report-identical-files   (ignores upper/lower case differences)
// diff -s aka --report-identical-files   (reports if files are the same) shows default output if files are different
// diff file1 file2  aka --normal       (default behavior)
// diff -y aka --side-by-side           (side by side format && prints common lines)
// diff -y --left-column                (..., prints only the left column of common lines)
// diff -y --suppress-common-lines ...  (side-by-side but doesn't show common lines)
// diff -c NUM aka --context=NUM        (shows NUM default 3 lines of copied context)
// diff -u NUM aka --unified=NUM        (shows NUM default 3 lines of unified context)
// y normal conflict
// (q -y ignores)
// (q -s works)
// (-v -q v overrides)
// -c -u conflict
// --normal and -left-column conflict
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define BUFLEN 256
#define MAXSTRINGS 1024

typedef struct para para;
struct para {
  char** base;
  int filesize;
  int start;
  int stop;
  char* firstline;
  char* secondline;
};

para* para_make(char* base[], int size, int start, int stop);
para* para_first(char* base[], int size);
para* para_next(para* p);
size_t para_filesize(para* p);
size_t para_size(para* p);
char** para_base(para* p);
char* para_info(para* p);
int   para_equal(para* p, para* q);
void para_print(para* p, void (*fp)(const char*));

para* para_make(char* base[], int filesize, int start, int stop) {
  para* p = (para*) malloc(sizeof(para));
  p->base = base;
  p->filesize = filesize;
  p->start = start;
  p->stop = stop;
  p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
  p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];
  return p;
}

para* para_first(char* base[], int size) {
  para* p = para_make(base, size, 0, -1);
  return para_next(p);
}

void para_destroy(para* p) { free(p); }

para* para_next(para* p) {
  if (p == NULL || p->stop == p->filesize) { return NULL; }

  int i;
  para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
  for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
  pnew->stop = i;

  return pnew;
}
size_t para_filesize(para* p) { return p == NULL ? 0 : p->filesize; }

size_t para_size(para* p) { return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; }

char** para_base(para* p) { return p->base; }

char* para_info(para* p) {
  static char buf[BUFLEN];   // static for a reason
  snprintf(buf, sizeof(buf), "base: %p, filesize: %d, start: %d, stop: %d\n",
                  p->base, p->filesize, p->start, p->stop);
  return buf;  // buf MUST be static
}

int para_equal(para* p, para* q) {
  if (p == NULL || q == NULL) { return 0; }
  if (para_size(p) != para_size(q)) { return 0; }
  int i = p->start, j = q->start, equal = 0;
  while ((equal = strcmp(p->base[i], q->base[i])) == 0) { ++i; ++j; }
  return equal;
}

void para_print(para* p, void (*fp)(const char*)) {
  if (p == NULL) { return; }
  for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i]); }
}

char* yesorno(int condition) { return condition == 0 ? "no" : "YES"; }

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

void printboth(const char* left_right) {
  char buf[BUFLEN];
  size_t len = strlen(left_right);
  if (len > 0) { strncpy(buf, left_right, len); }
  buf[len - 1] = '\0';
  printf("%-50s %s", buf, left_right);
}

#define HASHLEN 200
int hash(const char* s) {
  unsigned long code = 0;
  while (*s != '\0') {
    code = code * 31 + *s++;
  }
  return code % HASHLEN;
}
int showversion = 0;
int showbrief = 0;
int ignorecase = 0;
int shownormal = 0;
int showcontext = 0;
int showsidebyside = 0;
int showleftcolumn = 0;
int showunified = 0;
void setoption(const char* arg, const char* s, const char* t, int* value) {
  if (strcmp(arg, s) == 0 || (t != NULL && strcmp(arg, t) == 0)) {
    *value = 1;
  }
}
void init_options_files(int argc, const char* argv[]) {
  int cnt = 0;
  const char* files[2] = { NULL, NULL};
  while (argc-- > 0) {
    const char* arg = *argv;
    setoption(arg, "-v", "--version", &showversion);
    setoption(arg, "-q", "--brief", &showbrief);
    setoption(arg, "-i", "--ignorecase", &showversion);
    setoption(arg, "-s", "--report-identical-files", &showversion);
    setoption(arg, "--normal", NULL, &shownormal);
    setoption(arg, "-y", "--side-by-side", &showsidebyside);
    setoption(arg, "--suppress-common-lines", NULL, &showversion);
    setoption(arg, "-c", "--context", &showcontext);
    setoption(arg, "-u", "--showunified", &showunified);
  //   switch (hash(arg)) {
  //     case VERS: case VERSION: showversion = 1; break;
  //     default:
  //     if (cnt == 2) {
  //       printf("");
  //     }
  //     files [cnt++] = arg;
  //   }
  // }
  if (arg[0] != '-') {
    if (cnt == 2) {

    }
  } else { files[cnt++] = arg; }
    ++argv;
  }
  if (!showcontext && !showunified && !showsidebyside &&!showleftcolumn) {

  }
}
void differror();
int main(int argc, const char * argv[]) {
  char buf[BUFLEN];
  char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];

  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));

  char flagbuf[BUFLEN];
  memset(flagbuf, 0, sizeof(flagbuf));
  int vflag = 0, qflag = 0, iflag = 0, sflag = 0, nflag = 0, yflag = 0, lflag = 0, supflag = 0, cflag = 0, uflag = 0;
  if (argc > 1) {
    strcpy(flagbuf, argv[1]);
    for (int i = 2; i <= argc; ++i) {
      strcat(flagbuf, argv[i - 1]);
    }
    if (strstr(flagbuf, "-v") != NULL || strstr(flagbuf, "--version") != NULL) { vflag = 1; }
    if (strstr(flagbuf, "-q") != NULL || strstr(flagbuf, "--brief") != NULL) { qflag = 1; }
    if (strstr(flagbuf, "-i") != NULL || strstr(flagbuf, "--ignore-case") != NULL) { iflag = 1; }
    if (strstr(flagbuf, "-s") != NULL || strstr(flagbuf, "--report-identical-files") != NULL) { sflag = 1; }
    if (strstr(flagbuf, "-normal") != NULL) { nflag = 1; }
    if (strstr(flagbuf, "-y") != NULL || strstr(flagbuf, "--side-by-side") != NULL) { yflag = 1; }
    if (strstr(flagbuf, "--left-column") != NULL) { lflag = 1; }
    if (strstr(flagbuf, "--suppress-common-lines") != NULL) { supflag = 1; }
    if (strstr(flagbuf, "-c") != NULL || strstr(flagbuf, "--context") != NULL) { cflag = 1; }
    if (strstr(flagbuf, "-u") != NULL || strstr(flagbuf, "--unified") != NULL) { uflag = 1; }
  }
  if (vflag) { printf("version yes\n"); } else { printf("version no\n"); }
  if (qflag) { printf("brief yes\n"); } else { printf("brief no\n"); }
  if (iflag) { printf("ignore-case yes\n"); } else { printf("ignore-case no\n"); }
  if (sflag) { printf("report-identical-files yes\n"); } else { printf("report-identical-files no\n"); }
  if (nflag) { printf("normal yes\n"); } else { printf("normal no\n"); }
  if (yflag) { printf("side-by-side yes\n"); } else { printf("side-by-side no\n"); }
  if (lflag) { printf("left-column yes\n"); } else { printf("left-column no\n"); }
  if (supflag) { printf("suppress-common-lines yes\n"); } else { printf("suppress-common-lines no\n"); }
  if (cflag) { printf("context yes\n"); } else { printf("context no\n"); }
  if (uflag) { printf("unified yes\n"); } else { printf("unified no\n"); }
  // if (argc > 3) { fprintf(stderr, "Usage: ./diff file1 file2\n");  exit(ARGC_ERROR); }
  // para_printfile(strings1, count1, printleft);
  //para* p = para_first(strings1, count1);

  FILE *fin1 = openfile(argv[1], "r");
  FILE *fin2 = openfile(argv[2], "r");

  int count1 = 0, count2 = 0;
  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[count1++] = strdup(buf); }
  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[count2++] = strdup(buf); }

  para* p = para_first(strings1, count1);
  para* q = para_first(strings2, count2);
  // int equal = 0;

  while (p != NULL) {
    while (q != NULL && !para_equal(p, q)) {
      para_print(q, printright);
      q = para_next(q);
    }
    if (q != NULL && para_equal(p, q)) { para_print(p, printboth);
    } else { para_print(p, printleft); }
    p = para_next(p); q = para_next(q);
  }

  // if (para_equal(p, q)) { para_print(p, printboth); return 0;}

  // para_print(q, printright);
  // while ((q = para_next(q)) != NULL && !equal) {
  //   equal = para_equal(p, q);
  //   para_print(q, equal ? printboth : printright);
  // }
  //
  // while ((p = para_next(p)) != NULL) {
  //   equal = 0;
  //   while (((q = para_next(q)) != NULL && !equal)) {
  //     equal = para_equal(p, q);
  //     para_print(q, equal ? printboth : printright);
  //   }
  //   para_print(p, printleft);
  // }
//  printf("p is: %s", para_info(p));
//  printf("q is: %s", para_info(q));
//  para_print(p, printleft);
//  para_print(q, printright);
//  printf("p and q are equal: %s\n\n", yesorno(para_equal(p, q)));

  // printf("\nTODO: check line by line in a paragraph, using '|' for differences");
  // printf("\nTODO: this starter code does not yet handle printing all of fin1's paragraphs.");
  // printf("\nTODO: handle the rest of diff's options\n");
  // printf("\tAs Tolkien said it, '...and miles to go before I sleep.'\n\n");
  fclose(fin1);
  fclose(fin2);
  return 0;
}
