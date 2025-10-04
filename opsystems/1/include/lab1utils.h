#ifndef LAB1UTILS_H
#define LAB1UTILS_H

#include <windows.h>

// Функция потока
DWORD WINAPI ThreadFunction(LPVOID lpParam);

// Функция для вывода информации о приоритетах
void PrintPriorityInfo(HANDLE hProcess, HANDLE hThread);

#endif
