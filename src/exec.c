#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmd.h"

void execute_executable(char *arguments[], int arg_count) {
  pid_t pid = fork();
  if (pid == 0) {
    arguments[arg_count] = NULL;
    execvp(arguments[0], arguments);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    printf("%s: command not found", arguments[0]);
  }
}
