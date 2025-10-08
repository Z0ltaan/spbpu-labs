

// #include <linux/limits.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// // #include <windows.h>
//
// #define LAB_ARRAY_SIZE 3;
// LPCWSTR lpCommandLine[LAB_ARRAY_SIZE] = {
//     L"C:\\WINDOWS\\SYSTEM32\\NOTEPAD.EXE temp.txt",
//     L"C:\\WINDOWS\\sYSTEM32\\CALC.EXE", L"C:\\WINNT\\SYSTEM32\\CHARMAP.EXE"};
//
// int lab1__readProgramList(WCHAR **dest, int destSize, FILE *src) {
//   if (destSize <= 0)
//     return 1;
//   WCHAR *path = NULL;
//   ssize_t pos = 0;
//   size_t bufSize = 0;
//   for (int i = 0; i < destSize || pos < 0; ++i) {
//     pos = getline(&path, &bufSize, src);
//     path
//   }
//   return 0;
// }
//
// STARTUPINFO startInfo;
// PROCESS_INFORMATION processInfo;
//
// int main(int argc, char **argv) {
//   if (argc != 2) {
//     perror("Wrong args\n");
//     return -1;
//   }
//
//   FILE *file = fopen(argv[1], "r");
//   if (file == NULL) {
//     perror("Wrong filename\n");
//     return -2;
//   }
//
//   ZeroMemory(&startInfo, sizeof(STARTUPINFO));
//   startInfo.cb = sizeof(startInfo);
//
//   // WCHAR proc_name[256];
//   for (int i = 0; i < LAB_ARRAY_SIZE; i++) {
//     wcscpy_s(proc_name, wcslen(lpCommandLine[i]) + 1, lpCommandLine[i]);
//     printf("!!!%ls %d\n", lpCommandLine[i], wcslen(lpCommandLine[i]));
//
//     if (!CreateProcess(NULL, proc_name, NULL, NULL, FALSE,
//                        HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL,
//                        &startInfo, &processInfo)) {
//       fprintf(stderr, "CreateProcess failed on error %d\n", GetLastError());
//       ExitProcess(1);
//     }
//   }
//
//   CloseHandle(processInfo.hProcess);
//   CloseHandle(processInfo.hThread);
//
//   return 0;
// }
