#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "project_defs.h"
#include "task.h"

struct TaskQueueNode
{
  struct TaskQueueNode* next;
  TTask taskID;
};

typedef struct TaskQueueNode TTaskQueueNode;

typedef struct
{
  TTaskQueueNode* head;
  TTaskQueueNode* tail;
} TTaskQueue;

void
TaskQueueInit(TTaskQueue* queue);

bool
IsTaskQueueEmpty(TTaskQueue* queue);

void
TaskQueuePush(TTaskQueue* queue, TTaskQueueNode* node);

TTaskQueueNode*
TaskQueuePop(TTaskQueue* queue);

#endif
