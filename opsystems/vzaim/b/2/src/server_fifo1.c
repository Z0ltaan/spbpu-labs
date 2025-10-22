#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MAXLINE 4096

void server(int readfd, int writefd);

int main(int argc, char **argv) {
  int readfd, writefd;

  // Создаем именованные каналы (FIFO)
  if (mknod(FIFO1, S_IFIFO | 0666, 0) < 0) {
    printf("Server: can not create FIFO1\n");
    exit(1);
  }
  printf("Server: FIFO1 is created!\n");

  if (mknod(FIFO2, S_IFIFO | 0666, 0) < 0) {
    unlink(FIFO1);
    printf("Server: can not create FIFO2\n");
    exit(1);
  }
  printf("Server: FIFO2 is created!\n");

  // Открываем каналы
  if ((readfd = open(FIFO1, O_RDONLY)) < 0) {
    printf("Server: can not open FIFO1 for read\n");
    exit(1);
  }
  printf("Server: FIFO1 is opened for read and readfd=%d\n", readfd);

  if ((writefd = open(FIFO2, O_WRONLY)) < 0) {
    printf("Server: can not open FIFO2 for write\n");
    exit(1);
  }
  printf("Server: FIFO2 is opened for write and writefd=%d\n", writefd);

  // Запускаем сервер
  server(readfd, writefd);

  // Закрываем дескрипторы
  close(readfd);
  close(writefd);

  printf("Server is terminated!\n");
  exit(0);
}

void server(int readfd, int writefd) {
  char buff[MAXLINE];
  int n;

  printf("Server: waiting for client requests...\n");

  // Читаем запрос от клиента
  while ((n = read(readfd, buff, MAXLINE)) > 0) {
    buff[n] = '\0'; // Добавляем нулевой терминатор
    printf("Server: received request: %s", buff);

    // Эмулируем обработку запроса (просто преобразуем в верхний регистр)
    for (int i = 0; i < n; i++) {
      if (buff[i] >= 'a' && buff[i] <= 'z') {
        buff[i] = buff[i] - 'a' + 'A';
      }
    }

    // Отправляем ответ клиенту
    write(writefd, buff, n);
    printf("Server: sent response: %s", buff);
  }

  if (n < 0) {
    printf("Server: read error\n");
  }
}
