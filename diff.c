#include "diff.h"

void version(void) {
  printf("\ndiff (CSUF diffutils) 1.0.0\n");
  printf("Copyright (C) 2019 CSUF\n");
  printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
  printf("You may redistribute copies of this program\n");
  printf("under the terms of the GNU General Public License.\n");
  printf("Written by James Vu.\n");
}

void loadfiles(const char* filename1, const char* filename2) {
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));

  FILE *fin1 = openfile(filename1, "r");
  FILE *fin2 = openfile(filename2, "r");

  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[count1++] = strdup(buf); }  fclose(fin1);
  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[count2++] = strdup(buf); }  fclose(fin2);
}

void diff_output_conflict_error(void) {
  fprintf(stderr, "diff: conflicting output style options\n");
  fprintf(stderr, "diff: Try `diff --help' for more information.)\n");
  exit(CONFLICTING_OUTPUT_OPTIONS);
}

void setoption(const char* arg, const char* s, const char* t, int* value) {
  if ((strcmp(arg, s) == 0) || ((t != NULL && strcmp(arg, t) == 0))) {
    *value = 1;
  }
}

void init_options_files(int argc, const char* argv[]) {
  int cnt = 0;

  while (argc-- > 0) {
    const char* arg = *argv;
    setoption(arg, "-v",       "--version",                  &showversion);
    setoption(arg, "-q",       "--brief",                    &showbrief);
    setoption(arg, "-i",       "--ignore-case",              &ignorecase);
    setoption(arg, "-s",       "--report-identical-files",   &report_identical);
    setoption(arg, "--normal", NULL,                         &diffnormal);
    setoption(arg, "-y",       "--side-by-side",             &showsidebyside);
    setoption(arg, "--left-column", NULL,                    &showleftcolumn);
    setoption(arg, "--suppress-common-lines", NULL,          &suppresscommon);
    setoption(arg, "-c",       "--context",                  &showcontext);
    setoption(arg, "-u",       "showunified",                &showunified);
    if (arg[0] != '-') {
      if (cnt == 2) {
        fprintf(stderr, "apologies, this version of diff only handles two files\n");
        fprintf(stderr, "Usage: ./diff [options] file1 file2\n");
        exit(TOOMANYFILES_ERROR);
      } else { files[cnt++] = arg; }
    }
    ++argv;   // DEBUG only;  move increment up to top of switch at release
  }

  if (!showcontext && !showunified && !showsidebyside && !showleftcolumn) {
    diffnormal = 1;
  }

  if (showversion) { version();  exit(0); }

  if (((showsidebyside || showleftcolumn) && (diffnormal || showcontext || showunified)) ||
      (showcontext && showunified) || (diffnormal && (showcontext || showunified))) {

    diff_output_conflict_error();
  }

  loadfiles(files[0], files[1]);
}

void print_normal(para* p, para* q) {
  para* qlast = q;
  para* plast = p;
  int foundmatch = 0;
  while (p != NULL) {
    foundmatch = 0;
    if (q != NULL) {
      qlast = q;
      while (q != NULL && (foundmatch = para_equal(p, q, ignorecase)) == 0) { q = para_next(q); }
      q = qlast;
    }

    if (foundmatch) {
      while ((foundmatch = para_equal(p, q, ignorecase)) == 0) { q = para_next(q); }
      if (qlast->start+1 < q->start) {
        printf("%da%d,%d\n", p->start, qlast->start+1, q->start);
        q = qlast;
        while ((foundmatch = para_equal(p, q, ignorecase)) == 0) {
          para_print(q, printnormaladd);
          q = para_next(q);
          qlast = q;
        }
      }
      qlast = q;
      if (foundmatch == 2) {
        int i = p->start, j = q->start;
        if (ignorecase) {
          while (i < p->stop && j < p->stop && (stricmp(p->base[i], q->base[j]) == 0)) {
            ++i; ++j;
          }
        } else {
          while (i < p->stop && j < p->stop && (strcmp(p->base[i], q->base[j]) == 0)) {
            ++i; ++j;
          }
        }
        int istart = i, jstart = j;
        if (ignorecase) {
          while (i < p->stop && j < p->stop && (stricmp(p->base[i], q->base[j]) != 0)) {
            ++i; ++j;
          }
        } else {
          while (i < p->stop && j < p->stop && (strcmp(p->base[i], q->base[j]) != 0)) {
            ++i; ++j;
          }
        }
        if ((i - istart == 1) && (j - jstart == 1)) {
          printf("%dc%d\n", istart+1, jstart+1);
          printnormaldelete(p->base[i-1]);
          printf("---\n");
          printnormaladd(q->base[j-1]);
        } else {
          printf("%d,%dc%d,%d\n", istart+1, i, jstart+1, j);
          while (istart != i) {
            printnormaldelete(p->base[istart]);
            ++istart;
          }
          printf("---\n");
          while (jstart != j) {
            printnormaladd(q->base[jstart]);
            ++jstart;
          }
        }
      }
      plast = p;
      p = para_next(p);
      q = para_next(q);
    } else {
      if (q == NULL) {
        printf("%d,%dd%d\n", p->start, p->filesize, qlast->filesize);
        printnormaldelete("\n");
        while (p != NULL) {
          para_print(p, printnormaldelete);
          p = para_next(p);
        }
      } else {
        plast = p;
        while(p != NULL && (foundmatch = para_equal(p, qlast, ignorecase)) == 0) {
          if (para_next(p) == NULL) { break; }
          p = para_next(p);
        }
        if (plast->start+1 < p->start) {
          printf("%d,%dd%d\n", plast->start+1, p->start, qlast->start);
          p = plast;
          while(p != NULL && (foundmatch = para_equal(p, qlast, ignorecase)) == 0) {
            para_print(p, printnormaldelete);
            if (para_next(p) == NULL) { plast = p; }
            p = para_next(p);
          }
        }
      }
    }
  }
  if (q != NULL) {
    printf("%da%d,%d\n", plast->filesize, q->start, q->filesize);
    printnormaladd("\n");
    while (q != NULL) {
      para_print(q, printnormaladd);
      q = para_next(q);
    }
  }
}

int print_brief(para* p, para* q) {
  while (p != NULL || q != NULL) {
    if (para_equal(p, q, ignorecase) != 1) {
      printf("Files %s and %s differ\n", files[0], files[1]);
      return 1;
    }
    p = para_next(p);
    q = para_next(q);
  }
  return 0;
}

int print_identical(para* p, para* q) {
  while (p != NULL || q != NULL) {
    if (para_equal(p, q, ignorecase) != 1) { return 0; }
    p = para_next(p);
    q = para_next(q);
  }
  printf("Files %s and %s are identical\n", files[0], files[1]);
  return 1;
}

void print_sidebyside(para* p, para* q) {
  para* qlast = q;
  int foundmatch = 0;
  while (p != NULL) {
    qlast = q;
    foundmatch = 0;
    while (q != NULL && (foundmatch = para_equal(p, q, ignorecase)) == 0) {
      q = para_next(q);
    }
    q = qlast;
    if (foundmatch) {
      while ((foundmatch = para_equal(p, q, ignorecase)) == 0) {
        para_print(q, printright);
        q = para_next(q);
        qlast = q;
      }
      if (suppresscommon && foundmatch == 1) {
        p = para_next(p);
        q = para_next(q);
      } else {
        para_printboth(p, q, suppresscommon, showleftcolumn);
        p = para_next(p);
        q = para_next(q);
      }
    } else {
      if (q == NULL) { printleft("\n"); }
      para_print(p, printleft);
      p = para_next(p);
    }
  }
  while (q != NULL) {
    printright("\n");
    para_print(q, printright);
    q = para_next(q);
  }
}

void print_context(para* p, para* q) {
  printf("It looks too complicated.\n");
}

void print_unified(para* p, para* q) {
  printf("It looks too complicated.\n");
}

int main(int argc, const char * argv[]) {
  init_options_files(--argc, ++argv);

  para* p = para_first(strings1, count1);
  para* q = para_first(strings2, count2);

  if (showbrief)        { if (print_brief(p, q))     { return 0; }  }
  if (report_identical) { if (print_identical(p, q)) { return 0; } }
  if (diffnormal)       {     print_normal(p, q);      return 0; }
  if (showsidebyside)   {     print_sidebyside(p, q);  return 0; }
  if (showcontext)      {     print_context(p, q);     return 0; }
  if (showunified)      {     print_unified(p, q);     return 0; }

  return 0;
}
