#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_arguments(char command[], char *argv[]) {
  int argc = 0;
  char *token = strtok(command, " ");
  while (token != NULL && argc < MAX_ARGS - 1) {
    argv[argc++] = token;
    token = strtok(NULL, " ");
  }
  argv[argc] = NULL;
  return argc;
}

shell_commands parse_command(char command[]) {
  if (strcmp(command, "exit") == 0)
    return CMD_EXIT;
  if (strcmp(command, "echo") == 0)
    return CMD_ECHO;
  if (strcmp(command, "type") == 0)
    return CMD_TYPE;
  if (strcmp(command, "pwd") == 0)
    return CMD_PWD;
  if (strcmp(command, "cd") == 0)
    return CMD_CD;
  return NONE;
}

char *find_executable(char executable[]) {
  char *path_env = getenv("PATH");
  // this copies it into heap so I need to free it
  char *path = strdup(path_env);
  char *dir = strtok(path, ":");
  while (dir != NULL) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, executable);
    if (access(full_path, X_OK) == 0) {
      char *result = strdup(full_path);
      free(path);
      return result;
    }
    dir = strtok(NULL, ":");
  }
  free(path);
  return NULL;
}
