#include "global.h"
#include <setjmp.h>
#include <stdint.h>
#include "project_defs.h"
#include "queue.h"

// TODO: at least one another table (resource table)
TTaskControlBlock taskTable[MAX_TASK_NUMBER];
TTaskQueue readyQueues[MAX_PRIORITY_NUMBER];
jmp_buf schedulerContext;
TTask currentTask;

// TODO: такое себе как будто бы (на каждый раз)
static int8_t
_GetHighestPriority()
{
  int8_t highestPriority = HIGHEST_TASK_PRIORITY;
  while (highestPriority >= 0)
  {
    if (readyQueues[highestPriority].head)
    {
      break;
    }
    --highestPriority;
  }
  return highestPriority;
}

void
_Schedule()
{

  int8_t highestPriority = _GetHighestPriority();

  if (highestPriority < 0)
  {
    longjmp(schedulerContext, JUMP_TO_SHUTDOWN);
  }

  TTaskQueueNode* node = TaskQueuePop(&readyQueues[highestPriority]);
  TTask taskID = node->taskID;

  taskTable[taskID].state = TASK_STATE_RUNNING;

  currentTask = taskID;
  if (!taskTable[taskID].isStarted)
  {
    taskTable[taskID].isStarted = 1;
    taskTable[taskID].taskFunc();

    taskTable[taskID].state = TASK_STATE_SUSPENDED;

    currentTask = INVALID_TASK;
    longjmp(schedulerContext, JUMP_TO_SCHEDULER);
  }
  else
  {
    longjmp(taskTable[taskID].context, JUMP_TO_SCHEDULER);
  }
}
