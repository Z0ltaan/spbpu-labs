#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define PERM 0666
#define MAX_MSG_SIZE 256

struct message {
  long mtype;
  char mtext[MAX_MSG_SIZE];
  pid_t client_pid;
};

int main(int argc, char *argv[]) {
  key_t key;
  int msgid;
  struct message msg, response;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <message>\n", argv[0]);
    exit(1);
  }

  key = ftok(".", 'S');
  if (key == -1) {
    perror("ftok");
    exit(1);
  }

  msgid = msgget(key, PERM);
  if (msgid == -1) {
    perror("msgget");
    exit(1);
  }

  printf("Client started (PID: %d). Queue ID: %d\n", getpid(), msgid);

  msg.mtype = 1;
  msg.client_pid = getpid();
  strncpy(msg.mtext, argv[1], MAX_MSG_SIZE - 1);
  msg.mtext[MAX_MSG_SIZE - 1] = '\0';

  printf("Client sent message: %s\n", msg.mtext);

  if (msgsnd(msgid, &msg, sizeof(msg.mtext) + sizeof(msg.client_pid), 0) ==
      -1) {
    perror("msgsnd");
    exit(1);
  }

  if (msgrcv(msgid, &response,
             sizeof(response.mtext) + sizeof(response.client_pid), 2,
             0) == -1) {
    perror("msgrcv");
    exit(1);
  }

  printf("Client recieved response (PID: %d): %s\n", response.client_pid,
         response.mtext);

  return 0;
}
