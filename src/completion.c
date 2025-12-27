#include "cmd.h";
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

const char *command_strings[] = {"exit", "echo", "type", "pwd", "cd"};

char *command_generator(const char *input, int state) {
  static int index = 0;
  static int length;
  if (state == 0) {
    index = 0;
    length = strlen(input);
  }

  while (command_strings[index] != NULL) {
    if (strncmp(input, command_strings[index], length) == 0) {
      return strdup(command_strings[index++]);
    }
    index++;
  }
  return NULL;
}

char **my_completion(const char *text, int start, int end) {
  rl_completion_append_character = ' ';
  return rl_completion_matches(text, command_generator);
}
