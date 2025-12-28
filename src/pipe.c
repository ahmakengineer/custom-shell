#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int count_commands(char **argv) {
  int count = 1;
  for (int i = 0; argv[i]; i++) {
    if (strcmp(argv[i], "|") == 0)
      count++;
  }
  return count;
}

char ***split_pipeline(char **argv, int *n_cmds) {
  *n_cmds = count_commands(argv);
  char ***cmds = malloc(sizeof(char **) * (*n_cmds));

  int cmd_index = 0;
  int start = 0;

  for (int i = 0;; i++) {
    if (argv[i] == NULL || strcmp(argv[i], "|") == 0) {
      int len = i - start;
      cmds[cmd_index] = malloc(sizeof(char *) * (len + 1));

      for (int j = 0; j < len; j++)
        cmds[cmd_index][j] = argv[start + j];

      cmds[cmd_index][len] = NULL;
      cmd_index++;
      start = i + 1;
    }
    if (argv[i] == NULL)
      break;
  }
  return cmds;
}

void execute_pipeline(char **argv) {
  int n_cmds = count_commands(argv);
  char ***cmds = split_pipeline(argv, &n_cmds);
  int pipes[n_cmds - 1][2];

  // Create pipes
  for (int i = 0; i < n_cmds - 1; i++)
    pipe(pipes[i]);

  for (int i = 0; i < n_cmds; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      // CHILD

      // stdin
      if (i > 0)
        dup2(pipes[i - 1][0], STDIN_FILENO);

      // stdout
      if (i < n_cmds - 1)
        dup2(pipes[i][1], STDOUT_FILENO);

      // Close all pipes
      for (int j = 0; j < n_cmds - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }

      execvp(cmds[i][0], cmds[i]);
      perror("execvp");
      _exit(1);
    }
  }

  // Parent closes pipes
  for (int i = 0; i < n_cmds - 1; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  // Wait for children
  for (int i = 0; i < n_cmds; i++)
    wait(NULL);
}
