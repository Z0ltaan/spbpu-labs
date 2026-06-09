#ifndef GLOBAL_H
#define GLOBAL_H

#include <setjmp.h>
#include <stdint.h>
#include "project_defs.h"
#include "queue.h"

typedef enum
{
  TASK_STATE_SUSPENDED,
  TASK_STATE_READY,
  TASK_STATE_RUNNING,
  TASK_STATE_BLOCKED
} TTaskState;

typedef struct _TaskControlBlock
{
  void (*taskFunc)();
  TTaskState state;
  int8_t priority;
  jmp_buf context;
  TTaskQueueNode readyNode;
  uint8_t isStarted;
} TTaskControlBlock;

extern TTaskControlBlock taskTable[MAX_TASK_NUMBER];
extern TTaskQueue readyQueues[MAX_PRIORITY_NUMBER];
extern jmp_buf schedulerContext;
extern TTask currentTask;

void _Schedule();

#endif
