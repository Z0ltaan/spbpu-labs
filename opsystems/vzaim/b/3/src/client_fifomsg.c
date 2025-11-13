#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
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

void client(int readfd, int writefd);
ssize_t mesg_send(int fd, struct mymesg *mptr);
ssize_t mesg_recv(int fd, struct mymesg *mptr);
// void hndlr(int);

int main(int argc, char **argv) {
  int readfd, writefd;
  // signal(SIGINT, hndlr);

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
  client(readfd, writefd);
  close(readfd);
  close(writefd);
  if (unlink(FIFO1) < 0) {
    printf("Client: can delete FIFO1\n");
    exit(1);
  }
  printf("Client: FIFO1 is deleted!\n");
  if (unlink(FIFO2) < 0) {
    printf("Client: can delete FIFO2\n");
    exit(1);
  }
  printf("Client: FIFO2 is deleted!\n");
  printf("Client is terminated!\n");
  exit(0);
}

void client(int readfd, int writefd) {
  // char tempbuff[MAXMESGDATA + 1];
  struct mymesg msg;
  while (fgets(msg.mesg_data, MAXMESGDATA, stdin) != NULL) {
    msg.mesg_len = strlen(msg.mesg_data);

    mesg_send(writefd, &msg);
    printf("Client: sent to server: %s", msg.mesg_data);

    if (mesg_recv(readfd, &msg)) {
      printf("Client: recieved %s\n", msg.mesg_data);
    } else {
      printf("Client: server closed connection\n");
    }
  }
}

ssize_t mesg_send(int fd, struct mymesg *mptr) {
  return (write(fd, mptr, MESGHDRSIZE + mptr->mesg_len));
}

ssize_t mesg_recv(int fd, struct mymesg *mptr) {
  ssize_t n;
  size_t len;
  char tempbuff[MAXMESGDATA + 1];

  if ((n = read(fd, mptr, MESGHDRSIZE)) == 0)
    return (0); // end of file
  else if (n != MESGHDRSIZE) {
    printf("Client: error MESGHDRSIZE\n");
    return (0);
  }
  if ((len = mptr->mesg_len) > 0)
    if ((n = read(fd, mptr->mesg_data, len)) != len) {
      printf("Client: could not read message from server\n");
      return (0);
    }
  return (len);
}
