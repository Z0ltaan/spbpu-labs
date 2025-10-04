#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {

  int pid, ppid, status;
  pid = getpid();
  ppid = getppid();

  printf("\n\n FATHER PARAM: pid=%i ppid=%i\n", pid, ppid);

  if (fork() == 0)
    execl("son", "son", NULL);

  system("ps xf > ps_out.txt");
  wait(&status);
}
