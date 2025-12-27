#include "cmd.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // REPL
  while (1) {
    // priting shell placholder
    printf("$ ");

    // declarations
    char command[1024];
    char *argv[MAX_ARGS];

    // Get and parse user input
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';
    int argc = get_arguments(command, argv);
    shell_commands cmd = parse_command(argv[0]);

    // Handling commands
    switch (cmd) {
    case CMD_EXIT:
      return 0;
      break;
    case CMD_ECHO:
      for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
      }
      printf("\n");
      continue;
      break;
    case CMD_TYPE:
      if (parse_command(argv[1]) != NONE)
        printf("%s is a shell builtin\n", argv[1]);
      else
        check_for_executable(argv[1]);
      break;
    case CMD_PWD:
      printf("%s\n", getcwd(NULL, 0));
      break;
    default:
      execute_executable(argv, argc);
      break;
    }
  }
  return 0;
}
