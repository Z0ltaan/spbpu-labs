#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MAXLINE 4096

void client(int readfd, int writefd);

int main(int argc, char **argv) {
  int readfd, writefd;

  // Открываем каналы (сервер уже должен создать их)
  if ((writefd = open(FIFO1, O_WRONLY)) < 0) {
    printf("Client: can not open FIFO1 for write\n");
    exit(1);
  }
  printf("Client: FIFO1 is opened for write writefd=%d\n", writefd);

  if ((readfd = open(FIFO2, O_RDONLY)) < 0) {
    printf("Client: can not open FIFO2 for read\n");
    exit(1);
  }
  printf("Client: FIFO2 is opened for read readfd=%d\n", readfd);

  // Запускаем клиент
  client(readfd, writefd);

  // Закрываем дескрипторы
  close(readfd);
  close(writefd);

  printf("Client is terminated!\n");
  exit(0);
}

void client(int readfd, int writefd) {
  char buff[MAXLINE];
  int n;

  // Читаем данные из stdin и отправляем серверу
  printf("Client: enter messages (Ctrl+D to exit):\n");

  while (fgets(buff, MAXLINE, stdin) != NULL) {
    int len = strlen(buff);

    // Отправляем данные серверу
    write(writefd, buff, len);
    printf("Client: sent to server: %s", buff);

    // Читаем ответ от сервера
    n = read(readfd, buff, MAXLINE);
    if (n > 0) {
      buff[n] = '\0';
      printf("Client: received from server: %s", buff);
    } else {
      printf("Client: server closed connection\n");
      break;
    }
  }

  printf("Client: end of input\n");
}
