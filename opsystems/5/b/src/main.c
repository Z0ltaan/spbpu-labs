#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

volatile BOOL runFlag = TRUE;
HANDLE *threads = NULL;
int threadCount = 0;

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

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Wrong arg\n");
    return 1;
  }
  threadCount = atoi(argv[1]);
  int workTime = atoi(argv[2]);
  if (threadCount <= 0 || workTime <= 0) {
    printf("Wrong arg\n");
    return 1;
  }
  HANDLE hProcess = GetCurrentProcess();
  HANDLE hMainThread = GetCurrentThread();

  PrintPriorityInfo(hProcess, hMainThread);

  threads = (HANDLE *)malloc(threadCount * sizeof(HANDLE));
  int *threadNumbers = (int *)malloc(threadCount * sizeof(int));

  for (int i = 0; i < threadCount; i++) {
    threadNumbers[i] = i + 1;
    threads[i] =
        CreateThread(NULL, 0, ThreadFunction, &threadNumbers[i], 0, NULL);

    if (threads[i] == NULL) {
      printf("Thread creation error %d\n", i + 1);
      return 1;
    }

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

  printf("\nChanged to HIGH_PRIORITY_CLASS...\n");
  SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
  Sleep(2000);

  printf("Changed to NORMAL_PRIORITY_CLASS...\n");
  SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
  Sleep(2000);

  printf("Forbid dynamic priority change\n");
  SetProcessPriorityBoost(hProcess, FALSE);
  SetThreadPriorityBoost(hMainThread, FALSE);
  Sleep(2000);

  printf("Stop half of threads for 3 seconds\n");
  for (int i = 0; i < threadCount / 2; i++) {
    SuspendThread(threads[i]);
  }
  Sleep(3000);

  printf("Start stopped threads...\n");
  for (int i = 0; i < threadCount / 2; i++) {
    ResumeThread(threads[i]);
  }

  printf("Work time %d seconds...\n", workTime);
  time_t startTime = time(NULL);

  while (time(NULL) - startTime < workTime) {
    Sleep(1000);
    int remaining = workTime - (int)(time(NULL) - startTime);
    printf("remaining %d...\n", remaining);
  }

  printf("Stopping all the threads...\n");
  runFlag = FALSE;
  WaitForMultipleObjects(threadCount, threads, TRUE, 5000);

  PrintPriorityInfo(hProcess, hMainThread);

  for (int i = 0; i < threadCount; i++) {
    CloseHandle(threads[i]);
  }

  free(threads);
  free(threadNumbers);

  ExitProcess(0);
  return 0;
}
