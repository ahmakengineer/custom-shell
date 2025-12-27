#include "cmd.h";
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *command_strings[] = {"exit", "echo", "type", "pwd", "cd"};

char *command_generator(const char *input, int state) {
  static int index_cmd, index_dir;
  static char *dirs[64];
  static int n_dirs;
  static DIR *dp = NULL;
  static struct dirent *entry;
  static int length;

  if (state == 0) {
    index_cmd = 0;
    index_dir = 0;
    length = strlen(input);

    // split PATH once
    char *path_env = getenv("PATH");
    char path_copy[1024];
    strncpy(path_copy, path_env, sizeof(path_copy));
    char *token = strtok(path_copy, ":");
    n_dirs = 0;
    while (token && n_dirs < 64)
      dirs[n_dirs++] = token, token = strtok(NULL, ":");

    // close previous DIR if any
    if (dp) {
      closedir(dp);
      dp = NULL;
    }
  }

  // first try command_strings
  while (command_strings[index_cmd]) {
    if (strncmp(input, command_strings[index_cmd], length) == 0)
      return strdup(command_strings[index_cmd++]);
    index_cmd++;
  }

  // then try PATH
  while (index_dir < n_dirs) {
    if (!dp)
      dp = opendir(dirs[index_dir]);
    if (!dp) {
      index_dir++;
      continue;
    }

    while ((entry = readdir(dp)) != NULL) {
      if (strncmp(input, entry->d_name, length) == 0) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirs[index_dir],
                 entry->d_name);
        if (access(full_path, X_OK) == 0)
          return strdup(entry->d_name);
      }
    }
    closedir(dp);
    dp = NULL;
    index_dir++;
  }

  return NULL;
}
char **my_completion(const char *text, int start, int end) {
  rl_completion_append_character = ' ';
  return rl_completion_matches(text, command_generator);
}
