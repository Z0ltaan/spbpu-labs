#define MAXLINE 255
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void server(int, int);
void client(int, int);

int main(int argc, char **argv) {
  int pipe1[2], pipe2[2];
  pid_t childpid;

  if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    perror("pipe");
    exit(1);
  }

  if ((childpid = fork()) == 0) { // child (server)
    close(pipe1[1]);
    close(pipe2[0]);
    server(pipe1[0], pipe2[1]);
    exit(0);
  }
  // parent (client)
  close(pipe1[0]);
  close(pipe2[1]);
  client(pipe2[0], pipe1[1]);
  waitpid(childpid, NULL, 0);
  exit(0);
}

void client(int readfd, int writefd) {
  size_t len;
  ssize_t n;
  char buff[MAXLINE];

  // Read filename from stdin
  if (fgets(buff, MAXLINE, stdin) == NULL) {
    perror("fgets");
    return;
  }

  len = strlen(buff);
  if (len > 0 && buff[len - 1] == '\n')
    buff[--len] = '\0'; // Remove newline

  // Send filename to server
  write(writefd, buff, len);

  // Read response from server and print to stdout
  while ((n = read(readfd, buff, MAXLINE)) > 0) {
    write(STDOUT_FILENO, buff, n);
  }
}

void server(int readfd, int writefd) {
  int fd;
  ssize_t n;
  char buff[MAXLINE + 1];
  char err_msg[] = "Error: Cannot open file\n";

  // Read filename from client
  if ((n = read(readfd, buff, MAXLINE)) > 0) {
    buff[n] = '\0'; // Null-terminate the string

    printf("Server received filename: %s\n", buff);

    // Try to open the file
    if ((fd = open(buff, O_RDONLY)) < 0) {
      // Send error message
      write(writefd, err_msg, strlen(err_msg));
      printf("Server: Cannot open file '%s'\n", buff);
    } else {
      printf("Server opened file: %s\n", buff);
      // Send file contents
      while ((n = read(fd, buff, MAXLINE)) > 0) {
        write(writefd, buff, n);
      }
      close(fd);
    }
  }

  close(writefd); // Important: close write end to signal EOF to client
} // #define MAXLINE 255
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/fcntl.h>
// #include <sys/wait.h>
// #include <unistd.h>
//
// void server(int, int);
// void client(int, int);
//
// int main(int argc, char **argv) {
//   int pipe1[2], pipe2[2];
//   pid_t childpid;
//   pipe(pipe1);
//   pipe(pipe2);
//   if ((childpid = fork()) == 0) { // child
//     close(pipe1[1]);
//     close(pipe2[0]);
//     server(pipe1[0], pipe2[1]);
//     exit(0);
//   }
//   // parent
//   close(pipe1[0]);
//   close(pipe2[1]);
//   client(pipe2[0], pipe1[1]);
//   waitpid(childpid, NULL, 0);
//   exit(0);
// }
// void client(int readfd, int writefd) {
//   size_t len;
//   ssize_t n;
//   char buff[MAXLINE];
//   fgets(buff, MAXLINE, stdin);
//   printf("CLient read: %s\n", buff);
//   len = strlen(buff);
//   if (buff[len - 1] == '\n')
//     len--;
//   write(writefd, buff, len);
//   while ((n = read(readfd, buff, MAXLINE)) > 0)
//     // write(writefd, buff, n);
//     printf("Client said: %s\n", buff);
// }
// void server(int readfd, int writefd) {
//   int fd;
//   ssize_t n;
//   char buff[MAXLINE + 1];
//   if ((n = read(readfd, buff, MAXLINE)) == 0) {
//     printf("Server read: %s\n", buff);
//     buff[n] = '\0';
//   }
//   if ((fd = open(buff, O_RDONLY)) < 0) {
//     // ... n = strlen(buff);
//     printf("Server opened a file\n");
//     n = strlen(buff);
//     write(writefd, buff, n);
//   } else {
//     while ((n = read(fd, buff, MAXLINE)) > 0)
//       write(writefd, buff, n);
//     printf("Server wrote in pipe: %s\n", buff);
//     close(fd);
//   }
// }
