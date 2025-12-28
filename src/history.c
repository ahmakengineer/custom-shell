#include "cmd.h"
#include <readline/history.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void print_history(int count) {
  HIST_ENTRY **history = history_list();
  int start_at = history_length - count;
  start_at = start_at < 0 ? 0 : start_at;
  for (int i = start_at; i < history_length; i++) {
    printf("    %d  %s\n", i + 1, history[i]->line);
  }
}

void read_history_from_file(char *filename) { read_history(filename); }

void write_history_to_file(char *filename) { write_history(filename); }

void append_history_to_file(char *filename) {
  append_history(history_length, filename);
}
