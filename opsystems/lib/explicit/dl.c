#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_STRING 80

void invoke_method(char *lib, char *method, float argument) {
  void *dl_handle;
  float (*func)(float);
  char *error;

  dl_handle = dlopen(lib, RTLD_LAZY);
  if (!dl_handle) {
    printf("!!! %s\n", dlerror());
    return;
  }

  func = dlsym(dl_handle, method);
  error = dlerror();
  if (error != NULL) {
    printf("!!! %s\n", error);
    dlclose(dl_handle);
    return;
  }

  printf("Result: %f\n", (*func)(argument));
  dlclose(dl_handle);
}

int main(int argc, char *argv[]) {
  char line[MAX_STRING + 1];
  char lib[MAX_STRING + 1];
  char method[MAX_STRING + 1];
  float argument;

  while (1) {
    printf("> ");
    if (!fgets(line, MAX_STRING, stdin)) {
      break;
    }

    if (!strncmp(line, "end", 3))
      break;

    // Clear buffers
    lib[0] = method[0] = '\0';

    if (sscanf(line, "%s %s %f", lib, method, &argument) < 3) {
      printf("Usage: <library> <function> <argument>\n");
      printf("Example: /usr/lib/x86_64-linux-gnu/libm.so cosf 0.0\n");
      continue;
    }

    invoke_method(lib, method, argument);
  }

  return 0;
}
