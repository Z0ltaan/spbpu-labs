#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include <stdint.h>

#define MAX_TASK_NUMBER 32
#define MAX_PRIORITY_NUMBER 16
#define MAX_RESOURCE_NUMBER 16
#define MAX_EVENT_NUMBER 16

#define HIGHEST_TASK_PRIORITY (MAX_PRIORITY_NUMBER - 1)
#define LOWEST_TASK_PRIORITY 0

#define INVALID_TASK (-1)
#define INVALID_RESOURCE (-1)

#define JUMP_TO_SHUTDOWN 2
#define JUMP_TO_SCHEDULER 1

typedef int8_t TTask;
typedef int8_t TResource;

#endif
