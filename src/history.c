#include "cmd.h"
#include <readline/history.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void print_history() {
  HISTORY_STATE *history_state = history_get_history_state();
  HIST_ENTRY **history = history_list();

  for (int i = 0; i < history_state->length; i++) {
    printf("%d: %s\n", i + 1, history[i]->line);
  }
}
