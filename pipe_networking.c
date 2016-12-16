#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MESSAGE_BUFFER_SIZE 100

int client_handshake(int *server){
  mkfifo("public_pipe",0644);
  printf("[SERVER]: public_pipe made");
  
  char buffer[MESSAGE_BUFFER_SIZE];
  int pd = open("public_pipe", O_RDONLY);
  printf("[SERVER]: waiting for client");
  read(pd, buffer, sizeof(buffer));
  printf("[SERVER]: got message: %s",buffer);

  close(pd);
  system("rm public_pipe");
  printf("[SERVER]: public_pipe removed");

  int pd2 = open(buffer, O_WRONLY);
  char msg = "i gotchu";
  write(pd2, msg, sizeof(msg));
  printf("[SERVER]: sent message");
  return pd2;
}

int server_handshake(int *client){
  char pn = (char *)getpid();
  mkfifo(pn,0644);
  printf("[CLIENT]: private pipe made");
  
  int pd = open("public_pipe", O_WRONLY);
  write(pd, pn, sizeof(pn));
  int pd2 = open(pn, O_RDONLY);
  printf("[CLIENT]: sent message");

  char buffer[MESSAGE_BUFFER_SIZE];
  read(pd2, buffer, sizeof(buffer));
  printf("[CLIENT]: got message: %s",buffer);
  close(pd2);
  printf("[CLIENT]: removed pipe");
  
  
  char msg = "same";
  write(pd2, msg, sizeof(msg));
  return pd2;
}
