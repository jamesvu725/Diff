#include <stdlib.h>
#include <string.h>
#include "para.h"
#include "util.h"

#define BUFLEN 256

// make para
para* para_make(char* base[], int filesize, int start, int stop) {
  // allocate space for p
  para* p = (para*) malloc(sizeof(para));
  p->base = base;
  p->filesize = filesize;
  // start is start of paragraph
  p->start = start;
  p->stop = stop;
  // firstline if p->base[start]
  p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
  // secondline is p->base[start+1]
  p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];

  return p;
}
// first paragraph
para* para_first(char* base[], int size) {
  para* p = para_make(base, size, 0, -1);
  return para_next(p);
}
// free up space
void para_destroy(para* p) { free(p); }
// next paragraph
para* para_next(para* p) {
  // return NULL if p is null or stop = filesize
  if (p == NULL || p->stop == p->filesize) { return NULL; }

  int i;
  // next paragraph
  para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
  for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
  pnew->stop = i;

  if (pnew->start >= p->filesize) {
    free(pnew);
    pnew = NULL;
  }
  return pnew;
}
// return filesize
size_t para_filesize(para* p) { return p == NULL ? 0 : p->filesize; }
// return size
size_t para_size(para* p) { return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; }
// return base
char** para_base(para* p) { return p->base; }
// return paragraph info
char* para_info(para* p) {
  static char buf[BUFLEN];   // static for a reason
  snprintf(buf, sizeof(buf), "base: %p, filesize: %d, start: %d, stop: %d\n",
           p->base, p->filesize, p->start, p->stop);
  return buf;  // buf MUST be static
}
// check if para is equal
int para_equal(para* p, para* q, int ignorecase) {
  // return 0 if either is NULL
  if (p == NULL || q == NULL) { return 0; }
  // if paragraph size is unequal return 0
  if (para_size(p) != para_size(q)) { return 0; }
  // if start is larger than or equal to filesize return 0
  if (p->start >= p->filesize || q->start >= q->filesize) { return 0; }
  int i = p->start, j = q->start, equal = 0;
  // printf("%d %d\n", p->start, q->start);
  if (ignorecase) {
    while ((equal = stricmp(p->base[i], q->base[j])) == 0) {
      ++i; ++j;
      if (i >= p->stop || j >= q->stop) { break; }
    }
  } else {
    while ((equal = strcmp(p->base[i], q->base[j])) == 0) {
      ++i; ++j;
      if (i >= p->stop || j >= q->stop) { break; }
    }
  }
  // printf("%d\n", equal);
  if (equal == 0) { return 1; }
  return 2;
}
// print function for left and right
void para_print(para* p, void (*fp)(const char*)) {
  if (p == NULL) { return; }
  for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i]); }
}
// print both, checks for almost equal
void para_printboth(para* p, para* q) {
  if (p == NULL || q == NULL) { return; }
  for (int i = p->start, j = q->start; i <= p->stop && i != p->filesize &&
       j <= q->stop && j != q->filesize; ++i, ++j) {
    if (strcmp(p->base[i], q->base[j]) == 0) { printboth(p->base[i], q->base[j]); }
    else { printchange(p->base[i], q->base[j]); }
  }
}
// print file
void para_printfile(char* base[], int count, void (*fp)(const char*)) {
  para* p = para_first(base, count);
  while (p != NULL) {
    para_print(p, fp);
    p = para_next(p);
  }
  printline();
}

void para_printnormal(para* p, para* q, void(*fp)(const char*, const char*)) {
  if (p == NULL && q == NULL) { return; }
  if (p != NULL && q == NULL) {
    for (int i = p->start; i <= p->stop && i != p->filesize; ++i) {
      fp(p->base[i], NULL);
    }
  }
  if (p == NULL && q != NULL) {
    for (int i = q->start; i <= q->stop && i != q->filesize; ++i) {
      fp(NULL, q->base[i]);
    }
  }
  if (p != NULL && q != NULL) {
    for (int i = p->start, j = q->start; i <= p->stop && i != p->filesize &&
         j <= q->stop && j != q->filesize; ++i, ++j) {
      if (strcmp(p->base[i], q->base[j]) == 0) {
      } else {
        printf("%dc%d\n", i+1, j+1);
        fp(p->base[i], q->base[j]);
      }
    }
  }
}
