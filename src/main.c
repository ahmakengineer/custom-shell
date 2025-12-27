#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  FILE *fp = NULL;

  // REPL
  while (1) {
    if (fp != NULL) {
      fclose(fp);
      freopen("/dev/tty", "w", stdout);
      fp = NULL;
    }
    // priting shell placholder
    printf("$ ");

    // declarations
    char command[1024];

    // Get and parse user input
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    if (command[0] == '\0') {
      continue;
    }
    char **argv = parse_arguments(command);
    shell_commands cmd = parse_command(argv[0]);
    char *outfile = NULL;
    if (has_redirection(argv)) {
      for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "1>") == 0) {
          if (argv[i + 1] != NULL) {
            outfile = argv[i + 1]; // store filename
            argv[i] = NULL;        // remove '>' from argv
            argv[i + 1] = NULL;    // remove filename from argv
          } else {
            fprintf(stderr, "syntax error: expected filename after '>'\n");
          }
        }
      }
    }
    if (outfile != NULL) {
      fp = freopen(outfile, "w", stdout);
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
  }
  return 0;
}
