#include "cmd.h"
#include <process.h>
#include <stdio.h>
#include <sys/wait.h>

void execute_executable(char *arguments[], int arg_count) {
    pid_t pid = fork();
    if (pid == 0) {
      arguments[arg_count] = NULL;
      execvp(arguments[0], arguments);
  } else if(pid > 0) {
      wait(NULL);
} else {
    printf("%s: not found", arguments[0]);
    }
}
