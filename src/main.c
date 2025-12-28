#include "cmd.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  FILE *fp = NULL;
  rl_attempted_completion_function = my_completion;
  // REPL
  while (1) {
    if (fp != NULL) {
      fclose(fp);
      freopen("/dev/tty", "w", stdout);
      fp = NULL;
    }

    // Get and parse user input
    char *command = readline("$ ");
    command[strcspn(command, "\n")] = '\0';

    add_history(command);

    if (command[0] == '\0') {
      continue;
    }
    char **argv = parse_arguments(command);
    shell_commands cmd = parse_command(argv[0]);
    char *outfile = NULL;
    if (strstr(command, "|") != NULL) {
      execute_pipeline(argv);
      continue;
    }
    if (strstr(command, ">") != NULL) {
      fp = redirect(argv, outfile);
    }
    // Handling commands
    int code = handle_command(cmd, argv);
    if (code == 2) {
      return 0;
    }
    free(command);
  }
  return 0;
}
