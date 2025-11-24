#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEBUG
#include "debug.h"

void del_sem(int sig) {
  DEBUG_MSG("[DEBUG] %s...\n", "Unlinking semaphore \"mysem\"");
  sem_unlink("/mysem");
  DEBUG_DONE;
  exit(-1);
}

int main(int argc, char **argv) {
  sem_unlink("/mysem");
  signal(SIGINT, del_sem);
  DEBUG_INIT;
  int fd, i, nloop;
  char *ptr;
  sem_t *mutex;
  nloop = 10;
  DEBUG_MSG("[DEBUG] %s...", "Opening \"./filedata\"");
  // if ((fd = open("home/tvk/IPC/filedata", O_RDONLY)) < 0) {
  if ((fd = open("./filedata", O_RDWR)) < 0) {
    printf("Can not open file name\n");
    exit(1);
  }
  DEBUG_DONE;

  printf("File is opened!\n");

  DEBUG_MSG("[DEBUG] %s...", "Maping pages of memory");
  ptr = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    perror("Map fail");
    exit(-1);
  }
  DEBUG_DONE;

  close(fd);
  // mutex = sem_open(px_ipc_name("mysem"), O_CREAT | O_EXCL, 0);
  DEBUG_MSG("[DEBUG] %s...", "Creating semaphore \"mysem\"");
  mutex = sem_open("/mysem", O_CREAT | O_EXCL, -1);
  DEBUG_DONE;

  // DEBUG_MSG("[DEBUG] %s...", "Unlinking semaphore \"mysem\"");
  // sem_unlink("/mysem");
  // DEBUG_DONE;

  // exit(0);
  // sem_unlink(px_ipc_name("mysem"));

  // DEBUG_MSG("[DEBUG] %s...", "Unlinking semaphore \"mysem\"");
  // sem_unlink("/mysem");
  // DEBUG_DONE;

  setbuf(stdout, NULL);
  DEBUG_MSG("[DEBUG] %s...", "Creating child process");
  if (fork() == 0) {
    DEBUG_MSG("[DEBUG] %s...\n", "Writing child stuff");
    for (i = 0; i < nloop; i++) {
      DEBUG_MSG("[DEBUG] [CHILD] %s...\n", "Locking semaphore");
      sem_post(mutex);
      sleep(1);
      sem_wait(mutex);
      // DEBUG_DONE;

      printf("Child: %c\n", (*ptr)++);

      // DEBUG_MSG("[DEBUG] [CHILD] %s...\n", "Unlocking semaphore");
      DEBUG_DONE;
    }
    DEBUG_MSG("[DEBUG] ...%s\n", "done: Writing child stuff");
    return (0);
  }
  DEBUG_DONE;

  // DEBUG_MSG("[DEBUG] %s...\n", "Writing parent stuff");
  for (i = 0; i < nloop; i++) {
    DEBUG_MSG("[DEBUG] [PARENT] %s...\n", "Locking semaphore");
    sem_wait(mutex);
    sleep(1);
    sem_post(mutex);
    // printf("Parent: %c\n", (*ptr)++);
    // DEBUG_DONE;

    // DEBUG_MSG("[DEBUG] [PARENT] %s...\n", "Unlocking semaphore");
    // DEBUG_DONE;
  }
  DEBUG_MSG("[DEBUG] ...%s\n", "done: Writing parent stuff");

  DEBUG_MSG("[DEBUG] %s...", "Unlinking semaphore \"mysem\"");
  sem_unlink("/mysem");
  DEBUG_DONE;

  exit(0);
}
