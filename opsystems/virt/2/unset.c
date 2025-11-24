#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DEBUG
#include "debug.h"

int main() {
  DEBUG_MSG("[DEBUG] %s...", "Creating semaphore \"mysem\"");
  sem_t* mutex = sem_open("/mysem", O_CREAT | O_EXCL, 0);
  DEBUG_DONE;

  DEBUG_MSG("[DEBUG] %s...", "Unlinking semaphore \"mysem\"");
  sem_unlink("/mysem");
  DEBUG_DONE;

  exit(0);
}
