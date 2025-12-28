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

    switch (current) {

    // Whitespace → end token
    case ' ':
    case '\t':
    case '\n':
      if (token_cursor > 0) {
        token[token_cursor] = '\0';
        argv[index++] = strdup(token);
        token_cursor = 0;
      }
      cursor++;
      break;
    case '\\':
      cursor++;
      current = input[cursor];
      token[token_cursor++] = current;
      cursor++;
      break;
    case '"':
      cursor++;
      if (input[cursor + 1] == '"') {
        cursor++;
        continue;
      }

      while (input[cursor] != '\0') {
        if (input[cursor] == '\\') {
          if (input[cursor + 1] == '"' || input[cursor + 1] == '\\' ||
              input[cursor + 1] == '$' || input[cursor + 1] == '`') {
            cursor++;
            token[token_cursor++] = input[cursor++];
            continue;
          }
        }
        if (input[cursor] == '"') {
          // Escaped single quote: ''
          if (input[cursor + 1] == '"') {
            cursor += 2;
            continue;
          }
          break; // closing quote
        }
        token[token_cursor++] = input[cursor++];
      }

      // Skip closing quote if present
      if (input[cursor] == '"')
        cursor++;
      break;

    // Single-quoted token
    case '\'':
      // Skip opening quote
      cursor++;
      if (input[cursor + 1] == '\'') {
        cursor++;
        continue;
      }

      while (input[cursor] != '\0') {
        if (input[cursor] == '\'') {
          // Escaped single quote: ''
          if (input[cursor + 1] == '\'') {
            cursor += 2;
            continue;
          }
          break; // closing quote
        }
        token[token_cursor++] = input[cursor++];
      }

      // Skip closing quote if present
      if (input[cursor] == '\'')
        cursor++;
      break;

    // Normal character
    default:
      token[token_cursor++] = current;
      cursor++;
      break;
    }
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

int handle_command(shell_commands cmd, char **argv) {
  switch (cmd) {
  case CMD_EXIT:
    return 0;
    break;
  case CMD_ECHO:
    for (int i = 1; argv[i] != NULL; i++) {
      if (i > 1) {
        printf(" ");
      }
      printf("%s", argv[i]);
    }
    printf("\n");
    return 0;
  case CMD_TYPE:
    if (parse_command(argv[1]) != NONE)
      printf("%s is a shell builtin\n", argv[1]);
    else
      check_for_executable(argv[1]);
    break;
  case CMD_PWD:
    printf("%s\n", getcwd(NULL, 0));
    break;
  case CMD_CD:
    char *home = getenv("HOME");
    if (strstr(argv[1], "~") != NULL) {
      char *new = malloc(strlen(home) + strlen(argv[1]));
      strcpy(new, home);
      strcat(new, argv[1] + 1);
      argv[1] = new;
    }

    if (chdir(argv[1]) == -1) {
      printf("cd: %s: No such file or directory\n", argv[1]);
      break;
    }
    break;
  default:
    execute_executable(argv);
    break;
  }
  return 0;
}
