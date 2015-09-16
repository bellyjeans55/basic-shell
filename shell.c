/*
 * Author: Jonathan Kay
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define INPUT_BUFFER_SIZE 1024 //number chars first allocated for input
#define ARGS_BUFFER_SIZE 32 //number of argument pointers first allocated
#define ARGS_DELIMITER " \t\n\r\v\f"

/*
 * This is a simple *nix shell. It interpretes most standard
 * commands and uses fork and exec to create child processes where the
 * commands themselves execute.
 */

char** parameterize(char *input) {
  // tokenizes the input line and returns a list of args
  int current_buffer_size = ARGS_BUFFER_SIZE;
  char **args = malloc(current_buffer_size * sizeof(char*));
  int i = 0;

  // begin parsing, token holds first arg
  char *token = strtok(input, ARGS_DELIMITER);
  while (token) {
    args[i++] = token;
    token = strtok(NULL, ARGS_DELIMITER); // get next token
    if (i >= current_buffer_size) {
      // if more tokens than buffer can hold, allocate more space
      current_buffer_size += ARGS_BUFFER_SIZE;
      args = realloc(args, current_buffer_size * sizeof(char*));
    }
  }
  args[i] = NULL;
  return args;
}

/*
int exists_in_path(char *filename) {
  // returns "boolean" for if specified file exists in $PATH
  char* path = getenv("PATH");
  struct stat buffer;
  char *full_path;

       TODO: Strtok breaks my tokens for some reason. Find out what's up
       If strtok is called now, execvp will be unable
       to find any files/directories (even if they do
       exist in the path). The tokens are not modified in any way,
       but the exact same tokens parsed with strtok being called here
       will not work when they otherwise would have.
  //////////////////////////////////
  char *subpath = strtok(path, ":");
  //////////////////////////////////


  while(subpath != NULL) {
    int pathsize = strlen(path) + strlen(subpath);
    full_path = malloc(sizeof(char*) * pathsize);
    sprintf(full_path, "%s/%s", subpath, filename);
    if (stat(full_path, &buffer) == 0) {
      return 1;
    }
    subpath = strtok(NULL, ":");
  }
  return 0;
}
*/

void execute(char *input) {
  // uses fork and exec to create a new process and attempt to execute
  // the user command
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) {
    fprintf(stderr, "Error forking child process\n");
    exit(EXIT_FAILURE);
  }
  if (pid == 0) {
    // child process
    char **args = parameterize(input);

    if (execvp(args[0], args) == -1) {
      fprintf(stderr, "%s: %s\n", args[0], strerror(errno));
      exit(EXIT_FAILURE);
    }
    free(args);
  } else {
    wait(&status);
  }
}

int main() {
  // main program loop, handles calls to necessary functions and frees memory
  // when done. continues until EOF reached or program forcibly terminated
  char *input;
  ssize_t buffer_size;

  printf("%% ");
  while (getline(&input, &buffer_size, stdin) != -1) {
    execute(input);
    printf("%% ");
  }
  free(input);
  exit(0);
}
