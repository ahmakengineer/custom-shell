#include "cmd.h"
#include <stdio.h>
#include <string.h>

FILE *redirect(char **argv, char *outfile) {
  int redirect = 0;
  int stdout_redirection = 0;
  int index;
  int isAppend = 0;
  FILE *fp = NULL;

  for (int i = 0; argv[i] != NULL; i++) {
    if (strstr(argv[i], ">") != NULL) {
      redirect = 1;
      index = i;
    }

    if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "1>") == 0) {
      stdout_redirection = 1;
    } else if (strcmp(argv[i], "2>") == 0) {
      stdout_redirection = 2;
    } else if (strcmp(argv[i], ">>") == 0 || strcmp(argv[i], "1>>") == 0) {
      isAppend = 1;
      stdout_redirection = 1;
    } else if (strcmp(argv[i], "2>>") == 0) {
      isAppend = 1;
      stdout_redirection = 2;
    }
  }
  if (redirect == 1) {
    argv[index] = NULL;        // remove '>' from argv
    outfile = argv[index + 1]; // store filename
    argv[index + 1] = NULL;    // remove filename from argv
    if (stdout_redirection == 1) {
      if (isAppend == 1) {
        fp = freopen(outfile, "a", stdout);
      } else {
        fp = freopen(outfile, "w", stdout);
      }
    } else if (stdout_redirection == 2) {
      if (isAppend == 1) {
        fp = freopen(outfile, "a", stderr);
      } else {
        fp = freopen(outfile, "w", stderr);
      }
    }
  }
  return fp; // found
}
