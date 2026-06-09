#include "task.h"
#include <stddef.h>
#include <stdint.h>
#include "global.h"
#include "project_defs.h"
#include "queue.h"

static int8_t
_CapturePriority(int8_t priority)
{
  if (priority > HIGHEST_TASK_PRIORITY)
  {
    return HIGHEST_TASK_PRIORITY;
  }
  else if (priority < LOWEST_TASK_PRIORITY)
  {
    return LOWEST_TASK_PRIORITY;
  }
  return priority;
}

int8_t
RegisterTask(TTask taskID, void (*taskFunc)(void), int8_t priority)
{
  if (taskID < 0 || taskID >= MAX_TASK_NUMBER)
  {
    return -1;
  }

  if (priority < 0 || priority >= MAX_PRIORITY_NUMBER)
  {
    priority = _CapturePriority(priority);
  }

  taskTable[taskID].taskFunc = taskFunc;
  taskTable[taskID].state = TASK_STATE_SUSPENDED;
  taskTable[taskID].priority = priority;
  taskTable[taskID].readyNode.taskID = taskID;
  taskTable[taskID].readyNode.next = NULL;
  taskTable[taskID].isStarted = 0;

  return 0;
}

void
ActivateTask(TTask task)
{
  if (task < 0 || task >= MAX_TASK_NUMBER)
    return;
  if (taskTable[task].state != TASK_STATE_SUSPENDED)
    return;

  taskTable[task].state = TASK_STATE_READY;
  taskTable[task].isStarted = 0;

  TaskQueuePush(&readyQueues[taskTable[task].priority],
                &taskTable[task].readyNode);
}

void
TerminateTask()
{
  if (currentTask == INVALID_TASK)
    return;

  taskTable[currentTask].state = TASK_STATE_SUSPENDED;
  currentTask = INVALID_TASK;

  longjmp(schedulerContext, JUMP_TO_SCHEDULER);
}

void
YeildTask()
{
  if (setjmp(taskTable[currentTask].context) == 0)
  {
    taskTable[currentTask].state = TASK_STATE_READY;
    TaskQueuePush(&readyQueues[taskTable[currentTask].priority],
                  &taskTable[currentTask].readyNode);
    currentTask = INVALID_TASK;
    longjmp(schedulerContext, JUMP_TO_SCHEDULER);
  }
}
