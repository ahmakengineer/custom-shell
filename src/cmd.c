#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

char **parse_arguments(char *input) {
  int cursor = 0;
  int index = 0;
  char token[256];
  int token_cursor = 0;

  char **argv = malloc(sizeof(char *) * (MAX_ARGS + 1));
  if (!argv)
    return NULL;

  while (input[cursor] != '\0') {
    char current = input[cursor];

    // Whitespace: end token
    if (current == ' ' || current == '\t' || current == '\n') {
      if (token_cursor > 0) {
        token[token_cursor] = '\0';
        argv[index++] = strdup(token);
        token_cursor = 0;
      }
      cursor++;
      continue;
    }

    // Single-quoted token
    if (current == '\'') {
      if (input[cursor + 1] == '\'') {
        cursor += 2;
        continue;
      }
      cursor++; // skip opening quote
      while (input[cursor] != '\0') {
        if (input[cursor] == '\'') {
          if (input[cursor + 1] == '\'') {
            cursor += 2;
          } else {
            break;
          }
        }
        token[token_cursor++] = input[cursor++];
      }
      if (input[cursor] == '\'')
        cursor++; // skip closing quote
      continue;
    }
    // Normal character
    token[token_cursor++] = current;
    cursor++;
  }

  // Final token
  if (token_cursor > 0) {
    token[token_cursor] = '\0';
    argv[index++] = strdup(token);
  }

  if (index == 0) {
    free(argv);
    return NULL;
  }

  argv[index] = NULL;
  return argv;
}

void move_cursor(int *cursor, int *size, char current, char *input) {
  if (cursor < size && current != '\0') {
    cursor++;
    current = input[*cursor];
  }
}
