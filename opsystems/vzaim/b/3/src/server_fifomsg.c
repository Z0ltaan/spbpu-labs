#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <threads.h>
#include <unistd.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MAXMESGDATA 4096
struct mymesg {
  long mesg_len;
  long mesg_type;
  char mesg_data[MAXMESGDATA];
};
#define MESGHDRSIZE 2 * sizeof(long)

ssize_t mesg_send(int fd, struct mymesg *mptr);
ssize_t mesg_recv(int fd, struct mymesg *mptr);
void server(int readfd, int writefd);
void hndlr(int sig); // обработчик сигнала SIGINT

int main(int argc, char **argv) {
  int readfd, writefd;
  signal(SIGINT, hndlr);
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
  server(readfd, writefd);
  unlink(FIFO1);
  unlink(FIFO2);
  exit(0);
}

void server(int readfd, int writefd) {
  struct mymesg msg;
  char tempbuff[MAXMESGDATA + 1];

  while (mesg_recv(readfd, &msg) > 0) {
    strncpy(tempbuff, msg.mesg_data, msg.mesg_len);
    tempbuff[msg.mesg_len] = '\0';
    printf("Server: received request: %s", tempbuff);

    mesg_send(writefd, &msg);
    strncpy(tempbuff, msg.mesg_data, msg.mesg_len);
    printf("Server: sent request: %s", tempbuff);
  }
};

void hndlr(int sig) // обработчик сигнала SIGINT
{
  unlink(FIFO1);
  unlink(FIFO2);
}

ssize_t mesg_send(int fd, struct mymesg *mptr) {
  // преобразуем регистр сообщения для разницы сообщений от сервера
  for (int i = 0; i < mptr->mesg_len; i++) {
    if (mptr->mesg_data[i] >= 'a' && mptr->mesg_data[i] <= 'z') {
      mptr->mesg_data[i] = mptr->mesg_data[i] - 'a' + 'A';
    }
  }

  return (write(fd, mptr, MESGHDRSIZE + mptr->mesg_len));
}

ssize_t mesg_recv(int fd, struct mymesg *mptr) {
  ssize_t n;
  if ((n = read(fd, mptr, MESGHDRSIZE)) == 0) {
    printf("Server: end of file\n");
    return (0);
  } else if (n != MESGHDRSIZE) {
    printf("Server: error MESGHDRSIZE\n");
    return (0);
  }

  size_t len;
  if ((len = mptr->mesg_len) > 0) {
    if ((n = read(fd, mptr->mesg_data, len)) != len) {
      printf("Server: can not read msg\n");
      return (0);
    }
  }
  return (len);
}
