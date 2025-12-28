#include <stdio.h>

typedef enum Commands {
  CMD_EXIT,
  CMD_ECHO,
  CMD_TYPE,
  CMD_PWD,
  CMD_CD,
  NONE
} shell_commands;

#define MAX_ARGS 10

#pragma once
shell_commands parse_command(char command[]);

#pragma once
char **parse_arguments(char *input);

#pragma once
FILE *redirect(char **argv, char *outfile);

#pragma once
void execute_pipeline(char **argv);

#pragma once
int get_arguments(char command[], char *argv[]);

#pragma once
char *find_executable(char executable[]);

#pragma once
void execute_executable(char *arguments[]);
// Type command
#pragma once
void check_for_executable(char command[]);

#pragma once
char **my_completion(const char *text, int start, int end);

#pragma once
int handle_command(shell_commands cmd, char **argv);
