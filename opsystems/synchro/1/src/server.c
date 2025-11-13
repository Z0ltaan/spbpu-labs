#include "shmem.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

Message *msgptr;
int shmid, semid;

void hndlr(int sig); // обработчик сигнала SIGINT

int main(int argc, char **argv) {
  key_t key;
  signal(SIGINT, hndlr);

  if (argc != 2) {
    printf("wrong number of args");
    exit(-1);
  }
  // получение идентификатора разделяемого ресурса как для семафора так и для
  // разделяемой памяти
  if ((key = ftok(argv[1], 'A')) < 0) {
    printf("Server: can't get a key\n");
    exit(-1);
  }
  // создание области разделяемой памяти
  if ((shmid = shmget(key, sizeof(Message), PERM | IPC_CREAT)) < 0) {
    printf("Server: can't create an area\n");
    exit(-1);
  }
  printf("Server: area is created\n");

  // присоединение области
  if ((msgptr = (Message *)shmat(shmid, 0, 0)) < 0) {
    printf("Server: error of joining\n");
    exit(-1);
  }
  printf("Server: area is joined\n");

  // создание группы из 2 семафоров
  // 1 – для синхронизации работы с разделяемой памятью
  // 2 – для синхронизации выполнения процессов
  if ((semid = semget(key, 2, PERM | IPC_CREAT)) < 0) {
    printf("Server: can't create a semaphore\n");
    exit(-1);
  }
  printf("Server: semaphores are created\n");

  while (1) {
    // ожидание начала работы клиента
    if (semop(semid, &proc_wait[0], 1) < 0) {
      printf("Server: execution complete\n");
      exit(-1);
    }
    printf("post proc wait\n");
    // ожидание завершения работы клиента с разделяемой памятью
    if (semop(semid, &mem_lock[0], 2) < 0) {
      printf("Server: can't execute a operation\n");
      exit(-1);
    }

    // вывод сообщения, записанного клиентом в разделяемую память
    printf("Server: read message\n%s", msgptr->buff);

    // запись сообщения в разделяемую память
    sprintf(msgptr->buff, "Message from server with PID = %d\n", getpid());
    // освобождение ресурса
    if (semop(semid, &mem_unlock[0], 1) < 0) {
      printf("Server: can't execute a operation\n");
      exit(-1);
    }
  }
}

void hndlr(int sig) // обработчик сигнала SIGINT
{
  // отключение от области разделяемой памяти
  if (shmdt(msgptr) < 0) {
    printf("Server: error\n");
    exit(-1);
  }
  // удаление созданных объектов
  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    printf("Server : can't delete area\n");
    exit(-1);
  }
  printf("Server: area is deleted\n");
  if (semctl(semid, 0, IPC_RMID) < 0) {
    printf("Server : can't delete semaphore\n");
    exit(-1);
  }
  printf("Server: semaphores are deleted\n");
}
