#include "cmd.h"
#include <readline/history.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void print_history(int count) {
  HISTORY_STATE *history_state = history_get_history_state();
  HIST_ENTRY **history = history_list();
  int start_at = history_state->length - count;
  start_at = start_at < 0 ? 0 : start_at;
  for (int i = start_at; i < history_state->length; i++) {
    printf("    %d  %s\n", i + 1, history[i]->line);
  }
}
