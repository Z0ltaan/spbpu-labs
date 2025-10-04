#include <stdio.h>
#include <unistd.h>

int main() {
  int pid, ppid;
  pid = getpid();
  ppid = getppid();
  printf("\n INITIAL SON PARAM: pid=%i ppid=%i \n", pid, ppid);

  sleep(2);
  printf("\n LATE SON PARAM: pid=%i ppid=%i \n", getpid(), getppid());
}
