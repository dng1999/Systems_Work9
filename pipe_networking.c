#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MESSAGE_BUFFER_SIZE 100

int client_handshake(int *server){
  int success = mkfifo("public_pipe",0644);
  if (success){
    printf("Error %d: %s\n", errno, strerr(errno));
    return -1;
  }
  char buffer[MESSAGE_BUFFER_SIZE];
  int pd = open("public_pipe", O_RDONLY);
  read(pd, buffer, sizeof(buffer));
  close(pd);
  int pd2 = open(buffer, O_WRONLY);
  char msg = "i gotchu";
  write(pd2, msg, sizeof(msg));
}

int server_handshake(int *client){
  int success = mkfifo("private_pipe",0644);
  if (success){
    printf("Error %d: %s\n", errno, strerr(errno));
    return -1;
  }
  int pd = open("public_pipe", O_WRONLY);
  char pn = "private_pipe";
  write(pd, pn, sizeof(pn));
  int pd2 = open("private_pipe", O_RDONLY);
  char buffer[MESSAGE_BUFFER_SIZE];
  read(pd2, buffer, sizeof(buffer));
  close(pd2);
  char msg = "same";
  write(pd2, msg, sizeof(msg));
}
