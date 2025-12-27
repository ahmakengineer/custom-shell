typedef enum Commands {
  CMD_EXIT,
  CMD_ECHO,
  CMD_TYPE,
  CMD_PWD,
  NONE
} shell_commands;
#define MAX_ARGS 10

#pragma once
shell_commands parse_command(char command[]);

#pragma once
int get_arguments(char command[], char *argv[]);

#pragma once
char *find_executable(char executable[]);

#pragma once
void execute_executable(char *arguments[], int arg_count);
// Type command
#pragma once
void check_for_executable(char command[]);
