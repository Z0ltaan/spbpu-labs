#include "lab1utils.h"
#include "windows.h"

extern int runFlag;

// Функция потока
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
  int threadNum = *(int *)lpParam;
  DWORD priority = GetThreadPriority(GetCurrentThread());

  while (runFlag) {
    printf("Thread %d (priority: %ld) is running\n", threadNum, priority);
    Sleep(300); // Задержка для наглядности
  }

  printf("thread %d stopped\n", threadNum);
  return 0;
}

// Функция для вывода информации о приоритетах
void PrintPriorityInfo(HANDLE hProcess, HANDLE hThread) {
  DWORD processPriority = GetPriorityClass(hProcess);
  int threadPriority = GetThreadPriority(hThread);
  BOOL priorityBoost;

  GetProcessPriorityBoost(hProcess, &priorityBoost);

  printf("thread priority: ");
  switch (threadPriority) {
  case THREAD_PRIORITY_IDLE:
    printf("THREAD_PRIORITY_IDLE\n");
    break;
  case THREAD_PRIORITY_LOWEST:
    printf("THREAD_PRIORITY_LOWEST\n");
    break;
  case THREAD_PRIORITY_BELOW_NORMAL:
    printf("THREAD_PRIORITY_BELOW_NORMAL\n");
    break;
  case THREAD_PRIORITY_NORMAL:
    printf("THREAD_PRIORITY_NORMAL\n");
    break;
  case THREAD_PRIORITY_ABOVE_NORMAL:
    printf("THREAD_PRIORITY_ABOVE_NORMAL\n");
    break;
  case THREAD_PRIORITY_HIGHEST:
    printf("THREAD_PRIORITY_HIGHEST\n");
    break;
  case THREAD_PRIORITY_TIME_CRITICAL:
    printf("THREAD_PRIORITY_TIME_CRITICAL\n");
    break;
  default:
    printf("unknown (%d)\n", threadPriority);
  }

  printf("dynamic priotiy change: %s\n",
         priorityBoost ? "Allowed" : "Resticted");
}
