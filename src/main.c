#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_ARGS 10

typedef enum Commands {
    CMD_EXIT,
    CMD_ECHO,
    CMD_TYPE,
    NONE
} shell_commands;

void check_for_executable(char command[]) {
    char *path = getenv("PATH");
    char *dir = strtok(path, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        printf("dir : %s, command : %s, full_path : %s", dir, command, full_path);

        if ( access(full_path, X_OK) == 0) {
            printf("%s is %s\n", command, full_path);
            return;
        }

        dir = strtok(NULL, ":");
    }
    printf("%s: not found\n", command);
}

shell_commands parse_command(char command[]){
    if(strcmp(command, "exit") == 0) return CMD_EXIT;
    if(strcmp(command, "echo") == 0) return CMD_ECHO;
    if(strcmp(command, "type") == 0) return CMD_TYPE;

    return NONE;
}
int get_arguments(char command[], char *argv[]) {
    int argc = 0;
    char *token = strtok(command, " ");
    while(token != NULL && argc < MAX_ARGS - 1){
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;
    return argc;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
      printf("$ ");
      char command[1024];
      char *argv[MAX_ARGS];
      fgets(command, sizeof(command), stdin);
      command[strcspn(command, "\n")] = '\0';
      int argc = get_arguments(command, argv);
      shell_commands cmd = parse_command(argv[0]);
      switch(cmd) {
        case CMD_EXIT :
            return 0;
            break;
        case CMD_ECHO :
            for(int i = 1; i < argc; i++){
                printf("%s ", argv[i]);
            }
            printf("\n");
            continue;
            break;
        case CMD_TYPE:
            if(parse_command(argv[1]) != NONE)
            printf("%s is a shell builtin\n", argv[1]);
            else
            check_for_executable(argv[1]);
            break;
        default:
            printf("%s: command not found\n", command);
            break;
      }

  }

  return 0;
}
