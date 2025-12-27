#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_for_executable(char command[]) {
  char *full_path = find_executable(command);
  if (full_path != NULL) {
    printf("%s is %s\n", command, full_path);
  } else {
    printf("%s: not found\n", command);
  }
  free(full_path);
}
