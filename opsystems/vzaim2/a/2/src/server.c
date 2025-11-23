#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
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

int msgid = -1;

// Обработчик сигнала SIGINT
void sigint_handler(int sig) {
  printf("\nServer got interrupted\n");

  if (msgid != -1) {
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
    } else {
      printf("message queue is deleted\n");
    }
  }

  exit(0);
}

int main() {
  key_t key;
  struct message msg;

  // Устанавливаем обработчик сигнала
  signal(SIGINT, sigint_handler);

  // Создаем ключ для очереди сообщений
  key = ftok(".", 'S');
  if (key == -1) {
    perror("ftok");
    exit(1);
  }

  // Создаем очередь сообщений
  msgid = msgget(key, PERM | IPC_CREAT);
  if (msgid == -1) {
    perror("msgget");
    exit(1);
  }

  printf("Server is running\n");

  while (1) {
    // Ожидаем сообщение от клиента (тип 1)
    printf("Waiting for message...\n");

    if (msgrcv(msgid, &msg, sizeof(msg.mtext) + sizeof(msg.client_pid), 1, 0) ==
        -1) {
      // if (errno == EINTR) {
      //   // Прервано сигналом
      //   continue;
      // }
      perror("msgrcv");
      kill(getpid(), SIGINT);
    }

    printf("Server got a message for client (PID: %d): %s\n", msg.client_pid,
           msg.mtext);

    // Подготавливаем ответ
    struct message response;
    response.mtype = 2; // Тип ответа
    response.client_pid = getpid();

    // Формируем текст ответа
    snprintf(response.mtext, MAX_MSG_SIZE, "Server's response: '%s'",
             msg.mtext);

    // Отправляем ответ клиенту
    if (msgsnd(msgid, &response,
               sizeof(response.mtext) + sizeof(response.client_pid), 0) == -1) {
      perror("msgsnd");
      kill(getpid(), SIGINT);
    }

    printf("Server sent a response: %s\n", response.mtext);
  }

  return 0;
}
