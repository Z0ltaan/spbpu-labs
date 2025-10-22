#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NBUFF 10
#define nitems 100

struct {
  int buff[NBUFF];
  int put_index;
  int get_index;
  sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char **argv) {
  pthread_t tid_produce, tid_consume;

  shared.put_index = 0;
  shared.get_index = 0;

  shared.mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0644, 1);
  shared.nempty = sem_open("/nempty", O_CREAT | O_EXCL, 0644, NBUFF);
  shared.nstored = sem_open("/nstored", O_CREAT | O_EXCL, 0644, 0);

  if (shared.mutex == SEM_FAILED || shared.nempty == SEM_FAILED ||
      shared.nstored == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  if (pthread_create(&tid_produce, NULL, produce, NULL)) {
    perror("pthread_create produce");
    sem_unlink("/mutex");
    sem_unlink("/nempty");
    sem_unlink("/nstored");

    sem_close(shared.mutex);
    sem_close(shared.nempty);
    sem_close(shared.nstored);
    exit(1);
  }
  if (pthread_create(&tid_consume, NULL, consume, NULL)) {
    perror("pthread_create consume");
    sem_unlink("/mutex");
    sem_unlink("/nempty");
    sem_unlink("/nstored");

    sem_close(shared.mutex);
    sem_close(shared.nempty);
    sem_close(shared.nstored);
    exit(1);
  }

  pthread_join(tid_produce, NULL);
  pthread_join(tid_consume, NULL);

  sem_unlink("/mutex");
  sem_unlink("/nempty");
  sem_unlink("/nstored");

  sem_close(shared.mutex);
  sem_close(shared.nempty);
  sem_close(shared.nstored);

  return (0);
}

void *produce(void *arg) {
  int i;
  for (i = 0; i < nitems; i++) {
    sem_wait(shared.nempty); // Ждем свободное место
    sem_wait(shared.mutex);  // Входим в критическую секцию

    // Помещаем данные в циклический буфер
    shared.buff[shared.put_index] = i;
    printf("Производитель: поместил %d в ячейку %d\n", i, shared.put_index);
    shared.put_index =
        (shared.put_index + 1) % NBUFF; // Циклическое перемещение

    sem_post(shared.mutex);   // Выходим из критической секции
    sem_post(shared.nstored); // Увеличиваем счетчик заполненных ячеек
  }
  return (NULL);
}

void *consume(void *arg) {
  int i, item;
  for (i = 0; i < nitems; i++) {
    sem_wait(shared.nstored); // Ждем заполненную ячейку
    sem_wait(shared.mutex);   // Входим в критическую секцию

    // Извлекаем данные из циклического буфера
    item = shared.buff[shared.get_index];
    printf("Потребитель: извлек %d из ячейки %d\n", item, shared.get_index);
    shared.get_index =
        (shared.get_index + 1) % NBUFF; // Циклическое перемещение

    sem_post(shared.mutex);  // Выходим из критической секции
    sem_post(shared.nempty); // Увеличиваем счетчик свободных ячеек
  }
  return (NULL);
}
