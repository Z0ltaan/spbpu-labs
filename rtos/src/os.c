#include "os.h"
#include <stdint.h>
#include "global.h"
#include "project_defs.h"
#include "queue.h"

void
StartOS(TTask task)
{
  for (int8_t i = 0; i < MAX_PRIORITY_NUMBER; ++i)
  {
    TaskQueueInit(&readyQueues[i]);
  }

  if (task >= 0 && task < MAX_TASK_NUMBER)
  {
    taskTable[task].state = TASK_STATE_READY;
    TaskQueuePush(&readyQueues[taskTable[task].priority],
                  &taskTable[task].readyNode);
  }

  if (setjmp(schedulerContext) == JUMP_TO_SHUTDOWN)
  {
    return;
  }

  for(;;)
  {
    _Schedule();
  }
}

void
ShutdownOS()
{
  longjmp(schedulerContext, JUMP_TO_SHUTDOWN);
}
