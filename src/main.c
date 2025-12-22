#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ARGS 10

int parse_command(char command[], char *argv[]) {
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

      if(strcmp(command, "exit") == 0) {
          break;
      }
      
      int argc = parse_command(command, argv);

      if(argc == 0) {
          continue;
      }

      if(strcmp(argv[0], "echo") == 0) {
          for(int i = 1; i < argc; i++){
              printf("%s ", argv[i]);
          }
          printf("\n");
          continue;
      }

      printf("%s: command not found\n", command);
  }

  return 0;
}
