#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int error(char *msg) {
  perror(msg);
  exit(-1);
}

void split(char *cmdArray[], char *buffer) {
  // Split buffer by given char (delimimiter)
  int j = 0;
  char *tmp;
  buffer[strcspn(buffer, "\n")] = 0;
  while ((tmp = (strtok_r(buffer, " ", &buffer)))) {
    cmdArray[j] = malloc(sizeof(*tmp));
    strcpy(cmdArray[j++], tmp);
  }
  cmdArray[j] = NULL;
}

void realese(char **cmdArray) {
  for (int i = 0; i < 10; i++)
    if (cmdArray[i] != NULL) {
      free(cmdArray[i]);
      cmdArray[i] = NULL;
    }
}

int main() {
  pid_t pid;
  int status;
  char buffer[256], path[256] = "/bin/";
  char *cmdArray[10] = {NULL};

  while (1) {
    printf("advShell: ");
    fgets(buffer, 255, stdin);

    if (*buffer == '\n')
      continue;
    if (strcmp(buffer, "exit\n") == 0)
      break;

    if ((pid = fork()) == -1)
      error("Fork error");

    if (pid == 0) {
      split(cmdArray, buffer);
      execve(cmdArray[0], cmdArray, NULL);
      execve(strcat(path, cmdArray[0]), cmdArray, NULL);
      exit(-1);
    }
    wait(&status);
    if (status != 0)
      printf("Not Supported\n");
    realese(cmdArray);
  }
  printf("Goodbye");
  return 0;
}
