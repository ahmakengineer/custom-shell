#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


char *find_executable(char executable[]) {
  char *path_env = getenv("PATH");
  // this copies it into heap so I need to free it
  char *path = strdup(path_env);
  char *dir = strtok(path, ":");
  while (dir != NULL) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, executable);

    if (access(full_path, X_OK) == 0) {
      printf("%s is %s\n", executable, full_path);
      char *result = strdup(full_path);
      free(path);
      return result;
    }
    dir = strtok(NULL, ":");
  }
  free(path);
  return NULL;
}

void check_for_executable(char command[]) {
    char *full_path = find_executable(command);
    if (full_path != NULL) {
      printf("%s is %s\n", command, full_path);
    } else {
      printf("%s: not found\n", command);
    }
}
