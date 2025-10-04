#include "lab1utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <windows.h>

volatile BOOL runFlag = TRUE;
HANDLE *threads = NULL;
int threadCount = 0;

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Использование: %s <количество_потоков> <время_работы_в_секундах>\n",
           argv[0]);
    return 1;
  }
  threadCount = atoi(argv[1]);
  int workTime = atoi(argv[2]);
  if (threadCount <= 0 || workTime <= 0) {
    printf("Неверные параметры\n");
    return 1;
  }
  HANDLE hProcess = GetCurrentProcess();
  HANDLE hMainThread = GetCurrentThread();

  // Вывод начальной информации о приоритетах
  PrintPriorityInfo(hProcess, hMainThread);

  // Создание потоков с разными приоритетами
  threads = (HANDLE *)malloc(threadCount * sizeof(HANDLE));
  int *threadNumbers = (int *)malloc(threadCount * sizeof(int));

  for (int i = 0; i < threadCount; i++) {
    threadNumbers[i] = i + 1;
    threads[i] = CreateThread(NULL, // Атрибуты безопасности по умолчанию
                              0,    // Размер стека по умолчанию
                              ThreadFunction,    // Функция потока
                              &threadNumbers[i], // Параметр
                              0,                 // Флаги создания
                              NULL               // ID потока
    );

    if (threads[i] == NULL) {
      printf("Ошибка создания потока %d\n", i + 1);
      return 1;
    }

    // Установка разных приоритетов для потоков
    if (i % 6 == 0)
      SetThreadPriority(threads[i], THREAD_PRIORITY_IDLE);
    else if (i % 6 == 1)
      SetThreadPriority(threads[i], THREAD_PRIORITY_LOWEST);
    else if (i % 6 == 2)
      SetThreadPriority(threads[i], THREAD_PRIORITY_BELOW_NORMAL);
    else if (i % 6 == 3)
      SetThreadPriority(threads[i], THREAD_PRIORITY_NORMAL);
    else if (i % 6 == 4)
      SetThreadPriority(threads[i], THREAD_PRIORITY_ABOVE_NORMAL);
    else if (i % 6 == 5)
      SetThreadPriority(threads[i], THREAD_PRIORITY_HIGHEST);
  }

  // Демонстрация изменения приоритетов процесса
  printf("\nИзменяем класс приоритета процесса на HIGH_PRIORITY_CLASS...\n");
  SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
  Sleep(2000);

  printf("Изменяем класс приоритета процесса на NORMAL_PRIORITY_CLASS...\n");
  SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
  Sleep(2000);

  // Демонстрация запрета динамического изменения приоритета
  printf("Запрещаем динамическое изменение приоритета...\n");
  SetProcessPriorityBoost(hProcess, FALSE);
  SetThreadPriorityBoost(hMainThread, FALSE);
  Sleep(2000);

  // Демонстрация приостановки и возобновления потоков
  printf("Приостанавливаем половину потоков на 3 секунды...\n");
  for (int i = 0; i < threadCount / 2; i++) {
    SuspendThread(threads[i]);
  }
  Sleep(3000);

  printf("Возобновляем приостановленные потоки...\n");
  for (int i = 0; i < threadCount / 2; i++) {
    ResumeThread(threads[i]);
  }

  // Ожидание истечения времени работы
  printf("Программа будет работать %d секунд...\n", workTime);
  time_t startTime = time(NULL);

  while (time(NULL) - startTime < workTime) {
    Sleep(1000);
    int remaining = workTime - (int)(time(NULL) - startTime);
    printf("Осталось %d секунд...\n", remaining);
  }

  // Завершение работы
  printf("Завершаем работу потоков...\n");
  runFlag = FALSE;

  // Ожидание завершения потоков
  WaitForMultipleObjects(threadCount, threads, TRUE, 5000);

  // Вывод финальной информации о приоритетах
  PrintPriorityInfo(hProcess, hMainThread);

  // Очистка ресурсов
  for (int i = 0; i < threadCount; i++) {
    CloseHandle(threads[i]);
  }

  free(threads);
  free(threadNumbers);

  printf("Программа завершена.\n");
  ExitProcess(0);
  return 0;
}
