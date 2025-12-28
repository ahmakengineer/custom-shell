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
      if (outfile != NULL) {
        fp = freopen(outfile, "w", stdout);
      }
    }
    // Handling commands
    switch (cmd) {
    case CMD_EXIT:
      return 0;
      break;
    case CMD_ECHO:
      for (int i = 1; argv[i] != NULL; i++) {
        printf("%s ", argv[i]);
      }
      printf("\n");
      continue;
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
    free(command);
  }
  return 0;
}
